#include "RepositoryActions.h"
#include "Mission.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <cstdlib>
#include <functional>
#include <cstdio>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
namespace fs=std::filesystem;
namespace {
int checks=0;
void check(bool value,const std::string& name){++checks;if(!value)throw std::runtime_error("FAIL: "+name);}
void rejects(const std::function<void()>& f,const std::string& name){bool rejected=false;try{f();}catch(const std::exception&){rejected=true;}check(rejected,name);}
void put(const fs::path& p,const std::string& s){std::ofstream out(p,std::ios::binary);out<<s;if(!out)throw std::runtime_error("fixture write failed");}
std::string read(const fs::path& p){std::ifstream in(p,std::ios::binary);return pu::readText(in);}
struct PathOverride {
    std::string saved=std::getenv("PATH")?std::getenv("PATH"):"";
    explicit PathOverride(const std::string& s){set(s);}
    static void set(const std::string& s){
#ifdef _WIN32
        _putenv_s("PATH",s.c_str());
#else
        setenv("PATH",s.c_str(),1);
#endif
    }
    ~PathOverride(){set(saved);}
};
}
int main(int argc,char** argv) {
    if(argc>=2 && std::string(argv[1])=="--child") {
#ifdef _WIN32
        // The argv probe is a byte protocol. CRT text mode expands LF to CRLF,
        // which would fail exact comparison even when argv is completely correct.
        // Set the CHILD stream before any output; keep process capture untouched.
        if(_setmode(_fileno(stdout),_O_BINARY)==-1) {
            std::cerr<<"Cannot put test-child stdout in binary mode\n";return 125;
        }
#endif
        if(argc>=3 && std::string(argv[2])=="fail"){std::cerr<<"harmless failure\n";return 7;}
        if(argc>=3 && std::string(argv[2])=="wait"){std::this_thread::sleep_for(std::chrono::seconds(5));return 0;}
        if(argc>=3 && std::string(argv[2])=="flood"){for(int i=0;i<1100000;++i)std::cout<<'x';return 0;}
        for(int i=2;i<argc;++i)std::cout<<argv[i]<<'\n';
        return 0;
    }
    fs::path scratch;
    try {
        check(argc==2,"repository path argument");const auto original=fs::canonical(argv[1]);
        const auto self=fs::absolute(argv[0]);
        scratch=fs::temp_directory_path()/("pu-u2 space & probe-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        check(fs::create_directory(scratch),"reserve isolated scratch");
        for(auto n:{"src/main.cpp","src/UI/AppUISetup.cpp","src/UI/AppUISetup.h","university/include/Mission.h","university/include/ProgressStore.h","university/src/Mission.cpp","university/src/ProgressStore.cpp","university/tests/engine_tests.cpp","course/university/missions/u1-boundaries.pum","course/university/pilot.puc","visualstudio/Pandata.bank.vcxproj"}) {
            auto to=scratch/n;fs::create_directories(to.parent_path());fs::copy_file(original/n,to);
        }
        pu::RepositoryActions actions(scratch);
        check(actions.files().size()==11 && actions.files()[9]=="src/Objects/ObjectTypes.h" && actions.files()[10]=="src/Objects/ObjectTypes.cpp","bounded relevant file list with exact reviewed learner slots");
        auto before=read(scratch/"src/UI/AppUISetup.cpp");
        check(actions.view(3).find("ObjectSpawner")!=std::string::npos,"view actual relevant source");
        check(read(scratch/"src/UI/AppUISetup.cpp")==before,"view preserves bytes");
        rejects([&]{actions.view(0);},"zero index refused");
        rejects([&]{actions.view(1000);},"non-allowlisted index refused");
        rejects([&]{actions.execute("run-check && echo unsafe");},"command injection refused");
        rejects([&]{actions.execute("git commit");},"mutation command refused");
        check(!fs::exists(scratch/"course/university/evidence"),"rejected commands produce no evidence/write");
        check(pu::terminalText("safe\x1b[2J")=="safe\\x1b[2J","terminal escape neutralized");
        check(pu::jsonString("a\"\n")=="\"a\\\"\\u000a\"","JSON escaping");
        const std::vector<std::string> literal{"--child","space value","a\"b","C:\\path with space\\","$(touch SHOULD_NOT_EXIST); & | %PATH%"};
        auto child=pu::runProcess(self,literal,scratch);
        const std::string expected="space value\na\"b\nC:\\path with space\\\n$(touch SHOULD_NOT_EXIST); & | %PATH%\n";
        if(!child.ok() || child.output!=expected) {
            put(scratch/"argv-probe-output.bin",child.output);
            std::cerr<<"argv probe: started="<<child.started<<" exit="<<child.exitCode
                     <<" timeout="<<child.timedOut<<" output_limit="<<child.outputLimited
                     <<" error="<<pu::jsonString(child.error)<<'\n'
                     <<"expected bytes ("<<expected.size()<<"): "<<pu::jsonString(expected)<<'\n'
                     <<"captured bytes ("<<child.output.size()<<"): "<<pu::jsonString(child.output)<<'\n';
        }
        check(child.ok() && child.output==expected,"argv boundaries and metacharacters are literal");
        check(!fs::exists(scratch/"SHOULD_NOT_EXIST"),"no shell expansion");
        auto fail=pu::runProcess(self,{"--child","fail"},scratch);check(fail.started && fail.exitCode==7 && fail.output.find("harmless failure")!=std::string::npos,"capture stderr and nonzero exit");
        auto timeout=pu::runProcess(self,{"--child","wait"},scratch,60);check(timeout.timedOut && !timeout.ok(),"bounded timeout");
        auto flood=pu::runProcess(self,{"--child","flood"},scratch);check(flood.outputLimited && !flood.ok() && flood.output.size()==1024*1024,"bounded captured output");
        auto nul=pu::runProcess(self,{std::string("bad\0arg",7)},scratch);check(!nul.started && !nul.ok(),"NUL argument refused");
        auto missing=pu::runProcess(scratch/"missing-tool.exe",{},scratch);check(!missing.ok(),"missing executable reports failure");
        {PathOverride noTools("");auto a=actions.execute("run-check");check(!a.ok() && !a.error.empty() && fs::is_regular_file(a.evidence),"missing compiler recorded");
         auto g=actions.execute("git-status");check(!g.ok() && !g.error.empty(),"missing Git useful error");}
        {PathOverride relative(".");rejects([&]{pu::findTool("git",scratch);},"relative tool search refused");}
        {PathOverride repoTool(scratch.string());fs::copy_file(self,scratch/"git");rejects([&]{pu::findTool("git",scratch);},"repository executable refused");}
        auto valid=actions.execute("run-check");check(valid.ok() && valid.commands.size()==2 && valid.commands[1].output.find("PASS: 44")!=std::string::npos,"real compile and engine check succeeds");
        check(fs::is_regular_file(valid.evidence) && read(valid.evidence).find("\"passed\":true")!=std::string::npos,"successful machine evidence saved");
        auto failed=actions.execute("failure-demo");check(!failed.ok() && failed.commands.size()==2 && failed.commands[0].ok() && failed.commands[1].exitCode!=0,"real check fails with harmless scratch fixture");
        check(read(failed.evidence).find("\"passed\":false")!=std::string::npos,"failure is not reported as passing");
        check(read(original/"course/university/missions/u1-boundaries.pum")==read(scratch/"course/university/missions/u1-boundaries.pum"),"failure fixture preserves original lesson");
        check(!fs::exists(scratch/"course/university/progress"),"repository actions never write U1 progress");
        fs::rename(scratch/"src/main.cpp",scratch/"src/main.saved");fs::create_directory(scratch/"src/main.cpp");
        rejects([&]{actions.view(1);},"nonregular relevant file refused");fs::remove(scratch/"src/main.cpp");
        std::error_code ec;fs::create_symlink(scratch/"src/main.saved",scratch/"src/main.cpp",ec);
        if(!ec){rejects([&]{actions.view(1);},"symlink relevant file refused");fs::remove(scratch/"src/main.cpp");}
        else std::cout<<"SKIP: symlink creation denied by host; Windows junction check remains manual\n";
        fs::rename(scratch/"src/main.saved",scratch/"src/main.cpp");
#ifndef _WIN32
        auto vs=actions.execute("open-vs");check(!vs.ok() && vs.error.find("Windows-only")!=std::string::npos,"non-Windows VS error");
#endif
        fs::path git;
        try {
#ifdef _WIN32
            git=pu::findTool("git.exe",scratch);
#else
            git=pu::findTool("git",scratch);
#endif
        }catch(const std::exception&){std::cout<<"SKIP: Git integration, Git unavailable\n";}
        if(!git.empty()) {
            auto absent=actions.execute("git-status");check(!absent.ok(),"unversioned folder reports error");
            check(pu::runProcess(git,{"init"},scratch).ok(),"initialize isolated test checkout only");
            put(scratch/"tracked.txt","before\n");put(scratch/"clean.txt","unchanged\n");
            check(pu::runProcess(git,{"add","--","tracked.txt","clean.txt"},scratch).ok(),"stage test fixture only");
            check(pu::runProcess(git,{"-c","user.name=U2 test","-c","user.email=u2@example.invalid","-c","commit.gpgsign=false","commit","-m","test fixture"},scratch).ok(),"commit isolated fixture only");
            const auto index=read(scratch/".git/index"),head=read(scratch/".git/HEAD");
            fs::last_write_time(scratch/"clean.txt",fs::last_write_time(scratch/"clean.txt")+std::chrono::seconds(5));
            put(scratch/"tracked.txt","after\n");auto status=actions.execute("git-status"),diff=actions.execute("git-diff");
            check(status.ok() && status.commands.back().output.find("tracked.txt")!=std::string::npos,"read-only Git status");
            check(diff.ok() && diff.commands[2].output.find("+after")!=std::string::npos,"read-only worktree diff");
            check(read(scratch/".git/index")==index && read(scratch/".git/HEAD")==head,"Git view preserves index and HEAD");
            check(pu::runProcess(git,{"add","--","tracked.txt"},scratch).ok(),"stage changed fixture only");
            const auto stagedIndex=read(scratch/".git/index");auto staged=actions.execute("git-diff");
            check(staged.ok() && staged.commands.back().output.find("+after")!=std::string::npos,"staged diff visibility");
            check(read(scratch/".git/index")==stagedIndex,"staged diff preserves index");
            check(pu::runProcess(git,{"config","filter.danger.clean","echo SHOULD_NOT_EXECUTE"},scratch).ok(),"configure filter in fixture only");
            auto refused=actions.execute("git-diff");check(!refused.ok() && refused.error.find("filter")!=std::string::npos,"configured clean filter refused");
        }
        auto evidence=scratch/"course/university/evidence/u2";
        fs::rename(evidence,scratch/"prior-evidence");fs::create_directory(scratch/"outside-output");
        ec.clear();fs::create_directory_symlink(scratch/"outside-output",evidence,ec);
        if(!ec){PathOverride noTools("");auto refused=actions.execute("git-status");check(!refused.ok() && refused.evidence.empty() && refused.error.find("Evidence save failed")!=std::string::npos,"linked evidence path refuses writes");check(fs::is_empty(scratch/"outside-output"),"no redirected evidence written");}
        else std::cout<<"SKIP: directory symlink creation denied; junction refusal remains manual\n";
        fs::remove_all(scratch);std::cout<<"PASS: "<<checks<<" U2 checks\n";return 0;
    }catch(const std::exception& e){std::cerr<<e.what()<<"\nScratch preserved: "<<scratch<<'\n';return 1;}
}
