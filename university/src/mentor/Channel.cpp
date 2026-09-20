#include "mentor/Channel.h"
#include "Mission.h"
#include <algorithm>
#include <chrono>
#include <thread>
#include <cstdlib>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cerrno>
#endif
namespace pu::mentor {
namespace {
using Clock=std::chrono::steady_clock;
struct Pipe final:Channel {
    Clock::time_point deadline;std::string pending;std::size_t received=0,sent=0;
#ifdef _WIN32
    HANDLE input=nullptr,output=nullptr,process=nullptr,job=nullptr;
    std::jthread watchdog;
    ~Pipe(){watchdog.request_stop();if(watchdog.joinable())watchdog.join();if(job){TerminateJobObject(job,1);CloseHandle(job);}if(process){WaitForSingleObject(process,2000);CloseHandle(process);}if(input)CloseHandle(input);if(output)CloseHandle(output);}
#else
    int input=-1,output=-1;pid_t child=-1;
    ~Pipe(){if(child>0){kill(-child,SIGKILL);kill(child,SIGKILL);while(waitpid(child,nullptr,0)<0&&errno==EINTR){}}if(input>=0)close(input);if(output>=0)close(output);}
#endif
    explicit Pipe(int ms):deadline(Clock::now()+std::chrono::milliseconds(ms)){}
    void check(){if(Clock::now()>=deadline)throw Error("Codex deadline reached; mentor stopped. Offline commands remain available.");}
    void send(const std::string& line)override {
        if(line.size()>16384 || line.find('\n')!=std::string::npos || (sent+=line.size())>131072)throw Error("Mentor request limit exceeded");
        auto data=line+'\n';std::size_t at=0;
        while(at<data.size()){check();
#ifdef _WIN32
            // Each request is bounded below the configured 64 KiB pipe buffer.
            DWORD n=0;if(!WriteFile(input,data.data()+at,static_cast<DWORD>(data.size()-at),&n,nullptr)||!n)throw Error("Codex input closed");
#else
            auto n=::send(input,data.data()+at,data.size()-at,MSG_NOSIGNAL|MSG_DONTWAIT);
            if(n<0&&(errno==EAGAIN||errno==EINTR)){std::this_thread::sleep_for(std::chrono::milliseconds(5));continue;}
            if(n<=0)throw Error("Codex input closed");
#endif
            at+=static_cast<std::size_t>(n);
        }
    }
    std::string receive()override {
        while(true){check();auto end=pending.find('\n');if(end!=std::string::npos){auto line=pending.substr(0,end);pending.erase(0,end+1);return line;}
            char buf[4096];
#ifdef _WIN32
            DWORD available=0,n=0;
            if(!PeekNamedPipe(output,nullptr,0,nullptr,&available,nullptr))throw Error("Codex exited or closed its output; no answer accepted");
            if(!available){if(WaitForSingleObject(process,0)==WAIT_OBJECT_0)throw Error("Codex exited before completing the interaction");std::this_thread::sleep_for(std::chrono::milliseconds(5));continue;}
            if(!ReadFile(output,buf,std::min<DWORD>(available,sizeof(buf)),&n,nullptr)||!n)throw Error("Codex output closed");
#else
            auto n=read(output,buf,sizeof(buf));if(n<0&&(errno==EAGAIN||errno==EINTR)){std::this_thread::sleep_for(std::chrono::milliseconds(5));continue;}if(n<=0)throw Error("Codex exited or sent truncated JSONL; no answer accepted");
#endif
            received+=static_cast<std::size_t>(n);if(received>1024*1024)throw Error("Codex output limit exceeded");pending.append(buf,static_cast<std::size_t>(n));
        }
    }
};
#ifdef _WIN32
std::wstring wide(const std::string& s){int n=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s.data(),static_cast<int>(s.size()),nullptr,0);if(!n&&!s.empty())throw Error("Invalid UTF-8 argument");std::wstring w(n,L' ');MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s.data(),static_cast<int>(s.size()),w.data(),n);return w;}
std::wstring quote(const std::wstring& s){std::wstring out=L"\"";std::size_t n=0;for(auto c:s){if(c=='\\'){++n;continue;}out.append(c=='"'?2*n+1:n,L'\\');n=0;out+=c;}out.append(2*n,L'\\');return out+L'"';}
struct Handle{HANDLE h=nullptr;~Handle(){if(h&&h!=INVALID_HANDLE_VALUE)CloseHandle(h);}};
#endif
}
std::unique_ptr<Channel> spawn(const std::filesystem::path& exe,const std::vector<std::string>& args,const std::filesystem::path& cwd,int ms){
    if(!exe.is_absolute()||!cwd.is_absolute()||ms<1)throw Error("Mentor process requires absolute paths and a deadline");
    for(auto& a:args)if(a.find('\0')!=std::string::npos)throw Error("NUL argument refused");
    auto p=std::make_unique<Pipe>(ms);
#ifdef _WIN32
    SECURITY_ATTRIBUTES sa{sizeof(sa),nullptr,TRUE};Handle childIn,childOut,null;
    if(!CreatePipe(&childIn.h,&p->input,&sa,65536)||!CreatePipe(&p->output,&childOut.h,&sa,65536))throw Error("Cannot create Codex pipes");
    if(!SetHandleInformation(p->input,HANDLE_FLAG_INHERIT,0)||!SetHandleInformation(p->output,HANDLE_FLAG_INHERIT,0))throw Error("Cannot isolate Codex pipes");
    null.h=CreateFileW(L"NUL",GENERIC_WRITE,FILE_SHARE_READ|FILE_SHARE_WRITE,&sa,OPEN_EXISTING,0,nullptr);
    p->job=CreateJobObjectW(nullptr,nullptr);JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};limits.BasicLimitInformation.LimitFlags=JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
    if(null.h==INVALID_HANDLE_VALUE||!p->job||!SetInformationJobObject(p->job,JobObjectExtendedLimitInformation,&limits,sizeof(limits)))throw Error("Cannot contain Codex child job");
    std::vector<wchar_t> env;
    // Only OS/profile discovery variables; no provider secrets, proxies, external auth, or tool config overrides.
    for(auto key:{L"APPDATA",L"LOCALAPPDATA",L"PATH",L"PATHEXT",L"SystemRoot",L"TEMP",L"TMP",L"USERPROFILE",L"WINDIR"}){
        DWORD n=GetEnvironmentVariableW(key,nullptr,0);if(!n)continue;std::wstring v(n,L'\0');GetEnvironmentVariableW(key,v.data(),n);v.resize(n-1);std::wstring s=std::wstring(key)+L'='+v;env.insert(env.end(),s.begin(),s.end());env.push_back(0);
    }env.push_back(0);
    STARTUPINFOW si{};si.cb=sizeof(si);si.dwFlags=STARTF_USESTDHANDLES;si.hStdInput=childIn.h;si.hStdOutput=childOut.h;si.hStdError=null.h;
    auto command=quote(exe.wstring());for(auto& a:args)command+=L' '+quote(wide(a));PROCESS_INFORMATION pi{};
    if(!CreateProcessW(exe.c_str(),command.data(),nullptr,nullptr,TRUE,CREATE_NO_WINDOW|CREATE_SUSPENDED|CREATE_UNICODE_ENVIRONMENT,env.data(),cwd.c_str(),&si,&pi))throw Error("Codex launch failed: Windows error "+std::to_string(GetLastError()));
    p->process=pi.hProcess;Handle thread{pi.hThread};
    if(!AssignProcessToJobObject(p->job,p->process)){TerminateProcess(p->process,1);throw Error("Codex job containment denied; mentor unavailable");}
    if(ResumeThread(thread.h)==static_cast<DWORD>(-1))throw Error("Codex process could not start");
    // A blocked anonymous-pipe write must also obey the lifetime bound.
    auto raw=p.get();
    p->watchdog=std::jthread([raw](std::stop_token stop){while(!stop.stop_requested()){
        if(Clock::now()>=raw->deadline){TerminateJobObject(raw->job,1);return;}
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }});
#else
    int in[2],out[2];if(socketpair(AF_UNIX,SOCK_STREAM,0,in))throw Error("Cannot create mentor input");
    if(pipe(out)){close(in[0]);close(in[1]);throw Error("Cannot create mentor output");}
    p->input=in[0];p->output=out[0];
    std::vector<std::string> env,values{exe.string()};values.insert(values.end(),args.begin(),args.end());
    for(auto key:{"HOME","PATH","LANG","TMPDIR","XDG_CONFIG_HOME","XDG_DATA_HOME","XDG_RUNTIME_DIR"})if(auto v=std::getenv(key))env.push_back(std::string(key)+'='+v);
    std::vector<char*> ep,av;for(auto& v:env)ep.push_back(v.data());ep.push_back(nullptr);for(auto& v:values)av.push_back(v.data());av.push_back(nullptr);
    p->child=fork();if(p->child==0){setpgid(0,0);close(in[0]);close(out[0]);dup2(in[1],0);dup2(out[1],1);close(in[1]);close(out[1]);int null=open("/dev/null",O_WRONLY);if(null>=0){dup2(null,2);close(null);}if(chdir(cwd.c_str())==0)execve(exe.c_str(),av.data(),ep.data());_exit(127);}
    close(in[1]);close(out[1]);if(p->child<0)throw Error("Cannot fork Codex");setpgid(p->child,p->child);fcntl(p->output,F_SETFL,O_NONBLOCK);
#endif
    return p;
}
}
