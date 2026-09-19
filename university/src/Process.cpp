#include "RepositoryActions.h"
#include "Mission.h"
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <sstream>
#include <thread>
#include <iomanip>
#include <cwctype>
#ifdef _WIN32
#define NOMINMAX
#include <windows.h>
#else
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
extern char** environ;
#endif
namespace pu {
namespace fs = std::filesystem;
namespace {
constexpr std::size_t OutputLimit = 1024 * 1024;
bool under(const fs::path& p, const fs::path& root) {
    auto a=p.begin(),b=root.begin();
    for (; b!=root.end();++a,++b) {
        if(a==p.end()) return false;
#ifdef _WIN32
        if(_wcsicmp(a->c_str(),b->c_str())!=0) return false;
#else
        if(*a!=*b) return false;
#endif
    }
    return true;
}
#ifdef _WIN32
std::wstring wide(const std::string& s) {
    if (s.empty()) return {};
    const int count=MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s.data(),static_cast<int>(s.size()),nullptr,0);
    if (!count) throw Error("Invalid UTF-8 process argument");
    std::wstring out(static_cast<std::size_t>(count),L' ');
    MultiByteToWideChar(CP_UTF8,MB_ERR_INVALID_CHARS,s.data(),static_cast<int>(s.size()),out.data(),count); return out;
}
std::wstring quote(const std::wstring& value) {
    std::wstring out=L"\""; std::size_t slashes=0;
    for (wchar_t c:value) {
        if (c==L'\\') {++slashes;continue;}
        out.append(c==L'"'?slashes*2+1:slashes,L'\\');slashes=0;out+=c;
    }
    out.append(slashes*2,L'\\');return out+L'"';
}
struct Handle {
    HANDLE h=nullptr;
    explicit Handle(HANDLE value=nullptr):h(value){}
    ~Handle(){if(h && h!=INVALID_HANDLE_VALUE) CloseHandle(h);}
    Handle(const Handle&)=delete;
    Handle& operator=(const Handle&)=delete;
};
std::vector<wchar_t> environment() {
    std::vector<std::wstring> values;
    auto block=GetEnvironmentStringsW(); if(!block) throw Error("Cannot read child environment");
    for(auto p=block;*p;p+=wcslen(p)+1) {
        std::wstring s=p, upper=s; std::transform(upper.begin(),upper.end(),upper.begin(),towupper);
        if(!upper.starts_with(L"GIT_") && !upper.starts_with(L"CL=") && !upper.starts_with(L"_CL_=")) values.push_back(s);
    }
    FreeEnvironmentStringsW(block);
    for(auto s:{L"GIT_CONFIG_NOSYSTEM=1",L"GIT_CONFIG_GLOBAL=NUL",L"GIT_OPTIONAL_LOCKS=0",L"GIT_TERMINAL_PROMPT=0",L"GIT_ATTR_NOSYSTEM=1"}) values.emplace_back(s);
    std::sort(values.begin(),values.end(),[](const auto& a,const auto& b){return _wcsicmp(a.c_str(),b.c_str())<0;});
    std::vector<wchar_t> out;
    for(const auto& s:values){out.insert(out.end(),s.begin(),s.end());out.push_back(0);}out.push_back(0);return out;
}
#endif
}
std::string terminalText(const std::string& s) {
    std::string out;
    for(std::size_t i=0;i<s.size();++i) {
        const auto c=static_cast<unsigned char>(s[i]);
        if(c=='\r' && i+1<s.size() && s[i+1]=='\n')continue;
        if(c=='\n'||c=='\t'||(c>=32 && c<127)) out+=static_cast<char>(c);
        else { const char* h="0123456789abcdef";out+="\\x";out+=h[c>>4];out+=h[c&15]; }
    }
    return out;
}
std::string jsonString(const std::string& s) {
    std::ostringstream out;out<<'"';
    for(unsigned char c:s) {
        if(c=='"'||c=='\\') out<<'\\'<<static_cast<char>(c);
        else if(c<32||c>=127) out<<"\\u00"<<std::hex<<std::setw(2)<<std::setfill('0')<<static_cast<unsigned>(c);
        else out<<static_cast<char>(c);
    }
    out<<'"';return out.str();
}
void rejectLinks(const fs::path& p) {
    fs::path current;
    for(const auto& part:fs::absolute(p)) {
        current/=part;
#ifdef _WIN32
        const auto attr=GetFileAttributesW(current.c_str());
        if(attr!=INVALID_FILE_ATTRIBUTES && (attr&FILE_ATTRIBUTE_REPARSE_POINT)) throw Error("Reparse/symlink path refused: "+current.string());
#else
        if(fs::is_symlink(fs::symlink_status(current))) throw Error("Symlink path refused: "+current.string());
#endif
    }
}
fs::path findTool(const std::string& name,const fs::path& repository) {
    if(name.find_first_of("/\\:")!=std::string::npos) throw Error("Tool lookup requires a fixed basename");
#ifdef _WIN32
    const wchar_t* raw=_wgetenv(L"PATH"); std::wistringstream paths(raw?raw:L"");std::wstring entry;
    while(std::getline(paths,entry,L';')) {
        if(entry.size()>1 && entry.front()==L'"' && entry.back()==L'"') entry=entry.substr(1,entry.size()-2);
        fs::path base(entry);
#else
    const char* raw=std::getenv("PATH");std::istringstream paths(raw?raw:"");std::string entry;
    while(std::getline(paths,entry,':')) {
        fs::path base(entry);
#endif
        if(!base.is_absolute()) continue;
        std::error_code ec; auto candidate=fs::canonical(base/name,ec);
        if(ec || under(candidate,fs::canonical(repository)) || !fs::is_regular_file(candidate)) continue;
#ifndef _WIN32
        if(access(candidate.c_str(),X_OK)!=0) continue;
#endif
        return candidate;
    }
    throw Error("Required tool unavailable on an absolute PATH outside the repository: "+name+". Use the installed tool's Developer Command Prompt/PATH; no download or shell fallback was attempted.");
}
ProcessResult runProcess(const fs::path& exe,const std::vector<std::string>& args,const fs::path& cwd,int timeoutMs) {
    ProcessResult r;r.executable=exe;r.arguments=args;r.cwd=cwd;
    const auto begin=std::chrono::steady_clock::now();
    auto expired=[&]{return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-begin).count()>timeoutMs;};
    auto consume=[&](const char* p,std::size_t n){auto room=OutputLimit-r.output.size();r.output.append(p,std::min(room,n));if(n>room) r.outputLimited=true;};
    if(!exe.is_absolute() || !cwd.is_absolute() || timeoutMs<1) {r.error="Process requires absolute executable/cwd and positive deadline";return r;}
    for(const auto& a:args) if(a.find('\0')!=std::string::npos){r.error="NUL in argument refused";return r;}
#ifdef _WIN32
    try {
        SECURITY_ATTRIBUTES sa{sizeof(sa),nullptr,TRUE}; HANDLE readRaw=nullptr,writeRaw=nullptr;
        if(!CreatePipe(&readRaw,&writeRaw,&sa,0)) throw Error("CreatePipe failed");
        Handle read(readRaw),write(writeRaw);
        if(!SetHandleInformation(read.h,HANDLE_FLAG_INHERIT,0)) throw Error("SetHandleInformation failed");
        Handle input(CreateFileW(L"NUL",GENERIC_READ,FILE_SHARE_READ|FILE_SHARE_WRITE,&sa,OPEN_EXISTING,0,nullptr));
        Handle job(CreateJobObjectW(nullptr,nullptr));
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION limits{};limits.BasicLimitInformation.LimitFlags=JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        if(input.h==INVALID_HANDLE_VALUE || !job.h || !SetInformationJobObject(job.h,JobObjectExtendedLimitInformation,&limits,sizeof(limits))) throw Error("Cannot establish bounded child process");
        STARTUPINFOW start{};start.cb=sizeof(start);start.dwFlags=STARTF_USESTDHANDLES;start.hStdInput=input.h;start.hStdOutput=write.h;start.hStdError=write.h;
        std::wstring command=quote(exe.wstring());for(const auto& a:args) command+=L" "+quote(wide(a));
        auto env=environment();PROCESS_INFORMATION pi{};
        if(!CreateProcessW(exe.c_str(),command.data(),nullptr,nullptr,TRUE,CREATE_NO_WINDOW|CREATE_SUSPENDED|CREATE_UNICODE_ENVIRONMENT,env.data(),cwd.c_str(),&start,&pi)) throw Error("CreateProcess failed (Windows error "+std::to_string(GetLastError())+")");
        Handle process(pi.hProcess),thread(pi.hThread);
        if(!AssignProcessToJobObject(job.h,process.h)){TerminateProcess(process.h,1);WaitForSingleObject(process.h,INFINITE);throw Error("Cannot contain child process in job; action refused");}
        if(ResumeThread(thread.h)==static_cast<DWORD>(-1)){TerminateJobObject(job.h,1);throw Error("Cannot start child thread");}
        r.started=true;CloseHandle(write.h);write.h=nullptr;
        bool done=false;
        while(true) {
            DWORD available=0;
            if(PeekNamedPipe(read.h,nullptr,0,nullptr,&available,nullptr) && available) {
                char buffer[4096];DWORD got=0;
                if(ReadFile(read.h,buffer,std::min<DWORD>(available,sizeof(buffer)),&got,nullptr) && got) consume(buffer,got);
            } else if(done) break;
            if(expired() || r.outputLimited) {r.timedOut=expired();TerminateJobObject(job.h,1);WaitForSingleObject(process.h,INFINITE);break;}
            done=WaitForSingleObject(process.h,0)==WAIT_OBJECT_0;
            if(!available && !done) std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        DWORD code=0;if(GetExitCodeProcess(process.h,&code))r.exitCode=static_cast<int>(code);
    } catch(const std::exception& e){r.error=e.what();}
#else
    int pipes[2];
    if(pipe(pipes)!=0){r.error=std::strerror(errno);return r;}
    std::vector<std::string> env;
    for(char** p=environ;*p;++p) {std::string s=*p;if(!s.starts_with("GIT_") && !s.starts_with("CL=") && !s.starts_with("_CL_=")) env.push_back(s);}
    for(auto s:{"GIT_CONFIG_NOSYSTEM=1","GIT_CONFIG_GLOBAL=/dev/null","GIT_OPTIONAL_LOCKS=0","GIT_TERMINAL_PROMPT=0","GIT_ATTR_NOSYSTEM=1"})env.emplace_back(s);
    std::vector<char*> envp;for(auto& s:env)envp.push_back(s.data());envp.push_back(nullptr);
    std::vector<std::string> values{exe.string()};values.insert(values.end(),args.begin(),args.end());
    std::vector<char*> argv;for(auto& s:values)argv.push_back(s.data());argv.push_back(nullptr);
    const auto child=fork();
    if(child==0) {
        setpgid(0,0);close(pipes[0]);dup2(pipes[1],STDOUT_FILENO);dup2(pipes[1],STDERR_FILENO);close(pipes[1]);
        int null=open("/dev/null",O_RDONLY);if(null>=0){dup2(null,STDIN_FILENO);close(null);}
        if(chdir(cwd.c_str())==0)execve(exe.c_str(),argv.data(),envp.data());
        const char msg[]="Unable to launch executable or enter working directory\n";write(STDERR_FILENO,msg,sizeof(msg)-1);_exit(127);
    }
    close(pipes[1]);
    if(child<0){close(pipes[0]);r.error="fork failed";return r;}
    setpgid(child,child);r.started=true;fcntl(pipes[0],F_SETFL,O_NONBLOCK);
    bool done=false,eof=false;int status=0;
    while(!done || !eof) {
        char buffer[4096];const auto got=read(pipes[0],buffer,sizeof(buffer));
        if(got>0)consume(buffer,static_cast<std::size_t>(got));else if(got==0)eof=true;
        if(!done)done=waitpid(child,&status,WNOHANG)==child;
        if(expired() || r.outputLimited){r.timedOut=expired();kill(-child,SIGKILL);if(!done)waitpid(child,&status,0);done=true;break;}
        if(got<0)std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    kill(-child,SIGKILL);close(pipes[0]);
    if(WIFEXITED(status))r.exitCode=WEXITSTATUS(status);else if(WIFSIGNALED(status))r.exitCode=128+WTERMSIG(status);
#endif
    r.elapsedMs=std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()-begin).count();
    return r;
}
ProcessResult launchVisualStudio(const fs::path& exe,const fs::path& project,const fs::path& cwd) {
    ProcessResult r;r.executable=exe;r.arguments={project.string()};r.cwd=cwd;
#ifdef _WIN32
    std::wstring command=quote(exe.wstring())+L" "+quote(project.wstring());
    STARTUPINFOW start{};start.cb=sizeof(start);PROCESS_INFORMATION pi{};
    if(CreateProcessW(exe.c_str(),command.data(),nullptr,nullptr,FALSE,0,nullptr,cwd.c_str(),&start,&pi)) {
        CloseHandle(pi.hThread);CloseHandle(pi.hProcess);r.started=true;r.handedOff=true;r.exitCode=0;
        r.output="Launch handed to Visual Studio. This does not verify that the project loaded. Visual Studio may create its normal IDE/cache files.\n";
    } else r.error="Visual Studio launch failed (Windows error "+std::to_string(GetLastError())+")";
#else
    r.error="Open in Visual Studio is available on Windows only. Open visualstudio/Pandata.bank.vcxproj on your Windows machine.";
#endif
    return r;
}

}
