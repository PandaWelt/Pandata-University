#include "RepositoryActions.h"
#include "Mission.h"
#include <fstream>
#include <chrono>
#include <sstream>
#include <cstdlib>
#include <algorithm>

namespace pu {
namespace fs=std::filesystem;
namespace {
std::string utf8(const fs::path& p) {auto bytes=p.u8string();return std::string(bytes.begin(),bytes.end());}
fs::path safePath(const fs::path& root,const std::string& relative) {
    fs::path p(relative);
    if(p.is_absolute() || relative.empty() || relative.find_first_of("\\:")!=std::string::npos) throw Error("Unsafe repository path");
    for(const auto& part:p) if(part==".." || part==".") throw Error("Parent/dot path refused");
    auto full=root/p;rejectLinks(full);return full;
}
fs::path freshDirectory(const fs::path& root,const std::string& relative) {
    auto base=safePath(root,relative);fs::create_directories(base);
    const auto tick=std::chrono::system_clock::now().time_since_epoch().count();
    for(unsigned i=0;i<100;++i) {auto p=base/(std::to_string(tick)+"-"+std::to_string(i));if(fs::create_directory(p))return p;}
    throw Error("Cannot reserve a new output directory");
}
void put(const fs::path& file,const std::string& text) {
    std::ofstream out(file,std::ios::binary);if(!out)throw Error("Cannot write "+file.string());
    out<<text;out.flush();if(!out)throw Error("Write failed: "+file.string());
    out.close();if(!out)throw Error("Close failed: "+file.string());
}
std::vector<std::string> gitPrefix(){return {"--no-optional-locks","--no-pager","-c","core.fsmonitor=false","-c","core.untrackedCache=false","-c","diff.autoRefreshIndex=false","-c","color.ui=false","-c","core.quotePath=true"};}
}
bool ActionReport::ok() const {
    return error.empty() && !commands.empty() && std::all_of(commands.begin(),commands.end(),[](const auto& r){return r.ok();});
}
RepositoryActions::RepositoryActions(fs::path root) {
    rejectLinks(root);root_=fs::canonical(root);
    // Reviewed U2 scope. Mission content cannot grant additional file/command rights.
    files_={"src/main.cpp","src/UI/AppUISetup.h","src/UI/AppUISetup.cpp",
            "university/include/Mission.h","university/src/Mission.cpp",
            "university/include/ProgressStore.h","university/src/ProgressStore.cpp",
            "university/tests/engine_tests.cpp","visualstudio/Pandata.bank.vcxproj",
            "src/Objects/ObjectTypes.h","src/Objects/ObjectTypes.cpp"};
    if(!fs::is_regular_file(safePath(root_,"university/include/Mission.h")) || !fs::is_regular_file(safePath(root_,"src/UI/AppUISetup.cpp"))) throw Error("Launch University from the PandaBank repository root");
}
std::string RepositoryActions::view(std::size_t n) const {
    if(n==0 || n>files_.size())throw Error("File number is outside the reviewed allowlist. Use files.");
    auto file=safePath(root_,files_[n-1]);
    if(!fs::is_regular_file(file))throw Error("Relevant file missing or not regular: "+files_[n-1]);
    std::ifstream in(file,std::ios::binary);if(!in)throw Error("Cannot open relevant file: "+files_[n-1]);
    auto bytes=readText(in);std::istringstream lines(bytes);std::ostringstream result;std::string line;std::size_t number=0;
    while(std::getline(lines,line)) {
        if(!line.empty() && line.back()=='\r')line.pop_back();
        result<<++number<<" | "<<terminalText(line)<<'\n';
    }
    return files_[n-1]+" (read-only)\n"+result.str();
}
void RepositoryActions::save(ActionReport& report) const {
    auto dir=freshDirectory(root_,"course/university/evidence/u2");
    std::ostringstream out;out<<"{\n\"schema_version\":1,\n\"origin\":\"machine_observed\",\n\"action\":"<<jsonString(report.action)
        <<",\n\"repository\":"<<jsonString(utf8(root_))<<",\n\"recorded_unix_ms\":"
        <<std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count()
        <<",\n\"passed\":"<<(report.ok()?"true":"false")<<",\n\"intentional_failure_probe\":"<<(report.action=="failure-demo"?"true":"false")
        <<",\n\"error\":"<<jsonString(report.error)<<",\n\"output_encoding\":\"byte values escaped as U+0000..U+00FF; raw output-N.txt is authoritative\",\n\"commands\":[";
    for(std::size_t i=0;i<report.commands.size();++i) {
        const auto& c=report.commands[i];if(i)out<<',';
        out<<"\n{\"executable\":"<<jsonString(utf8(c.executable))<<",\"argv\":[";
        for(std::size_t a=0;a<c.arguments.size();++a){if(a)out<<',';out<<jsonString(c.arguments[a]);}
        out<<"],\"cwd\":"<<jsonString(utf8(c.cwd))<<",\"started\":"<<(c.started?"true":"false")
            <<",\"exit_status\":"<<(c.handedOff?"null":std::to_string(c.exitCode))<<",\"handed_off\":"<<(c.handedOff?"true":"false")
            <<",\"timed_out\":"<<(c.timedOut?"true":"false")<<",\"output_limited\":"<<(c.outputLimited?"true":"false")
            <<",\"elapsed_ms\":"<<c.elapsedMs<<",\"error\":"<<jsonString(c.error)<<",\"output\":"<<jsonString(c.output)<<'}';
        put(dir/("output-"+std::to_string(i+1)+".txt"),c.output);
    }
    out<<"]\n}\n";put(dir/"evidence.json.tmp",out.str());fs::rename(dir/"evidence.json.tmp",dir/"evidence.json");report.evidence=dir/"evidence.json";
}
ActionReport RepositoryActions::execute(const std::string& action) const {
    if(action!="run-check" && action!="failure-demo" && action!="git-status" && action!="git-diff" && action!="open-vs") throw Error("Unknown action. Only run-check, failure-demo, git-status, git-diff and open-vs are allowed.");
    ActionReport report;report.action=action;
    auto run=[&](const fs::path& exe,const std::vector<std::string>& args,const fs::path& cwd,int timeout=120000){
        report.commands.push_back(runProcess(exe,args,cwd,timeout));return report.commands.back().ok();};
    try {
        if(action=="run-check" || action=="failure-demo") {
            const auto stage=freshDirectory(root_,"build/university/u2-checks");
            const auto include=safePath(root_,"university/include");
            std::vector<std::string> sources;
            for(const auto& name:{"university/src/Mission.cpp","university/src/ProgressStore.cpp","university/tests/engine_tests.cpp"}) {
                auto p=safePath(root_,name);if(!fs::is_regular_file(p))throw Error("Check source missing: "+std::string(name));sources.push_back(utf8(p));
            }
            // Check transitive local headers before compiling trusted reviewed source.
            for(auto name:{"Mission.h","ProgressStore.h"})rejectLinks(include/name);
#ifdef _WIN32
            const auto compiler=findTool("cl.exe",root_);const auto binary=stage/"engine-tests.exe";
            std::vector<std::string> args{"/nologo","/std:c++20","/EHsc","/W4","/utf-8","/I"+utf8(include),"/Fe:"+utf8(binary)};
#else
            const auto compiler=findTool("g++",root_);const auto binary=stage/"engine-tests";
            std::vector<std::string> args{"-std=c++20","-Wall","-Wextra","-Werror","-pedantic","-I"+utf8(include),"-o",utf8(binary)};
#endif
            args.insert(args.end(),sources.begin(),sources.end());
            if(run(compiler,args,stage)) {
                auto mission=safePath(root_,"course/university/missions/u1-boundaries.pum");
                auto pilot=safePath(root_,"course/university/pilot.puc");
                if(action=="failure-demo") {mission=stage/"deliberately-invalid.pum";put(mission,"mission 999 \"invalid\"\n");}
                run(binary,{utf8(mission),utf8(pilot)},stage);
            }
        } else if(action=="git-status" || action=="git-diff") {
#ifdef _WIN32
            const auto git=findTool("git.exe",root_);
#else
            const auto git=findTool("git",root_);
#endif
            auto gitRun=[&](std::vector<std::string> more){auto args=gitPrefix();args.insert(args.end(),more.begin(),more.end());return run(git,args,root_,15000);};
            if(!gitRun({"rev-parse","--show-toplevel"}))throw Error("Git could not identify this working tree; use your existing checkout, not an unversioned extraction.");
            auto top=report.commands.back().output;while(!top.empty() && (top.back()=='\n'||top.back()=='\r'))top.pop_back();
            if(fs::canonical(fs::path(std::u8string(top.begin(),top.end())))!=root_)throw Error("Git root differs from University root; action refused.");
            if(!gitRun({"config","--name-only","--list","--includes"}))throw Error("Cannot audit effective Git configuration");
            // diff/status may invoke clean filters. Refuse those configurations rather than running user commands.
            std::istringstream settings(report.commands.back().output);std::string line;
            while(std::getline(settings,line))if(line.starts_with("filter."))throw Error("Git filter configuration detected; read-only action refused to avoid executing a filter. Use normal Git outside University or request a reviewed adapter.");
            if(action=="git-status")gitRun({"status","--short","--branch","--untracked-files=normal"});
            else {if(gitRun({"diff","--no-ext-diff","--no-textconv","--"}))gitRun({"diff","--cached","--no-ext-diff","--no-textconv","--"});}
        } else {
#ifdef _WIN32
            const auto raw=_wgetenv(L"ProgramFiles(x86)");
            if(!raw)throw Error("Visual Studio discovery unavailable: ProgramFiles(x86) missing");
            const auto where=fs::path(raw)/"Microsoft Visual Studio/Installer/vswhere.exe";
            if(!fs::is_regular_file(where))throw Error("Visual Studio discovery unavailable: vswhere.exe not installed. Open visualstudio/Pandata.bank.vcxproj manually.");
            if(!run(where,{"-latest","-products","*","-requires","Microsoft.VisualStudio.Component.CoreEditor","-property","installationPath","-utf8"},root_,15000))throw Error("Visual Studio discovery failed");
            auto found=report.commands.back().output;while(!found.empty() && (found.back()=='\r'||found.back()=='\n'))found.pop_back();
            if(found.empty())throw Error("No Visual Studio IDE found. Build Tools alone cannot open the project.");
            auto exe=fs::path(std::u8string(found.begin(),found.end()))/"Common7/IDE/devenv.exe";
            if(!exe.is_absolute() || !fs::is_regular_file(exe))throw Error("Visual Studio IDE executable unavailable");
            auto project=safePath(root_,"visualstudio/Pandata.bank.vcxproj");
            if(!fs::is_regular_file(project))throw Error("PandaBank Visual Studio project is missing");
            report.commands.push_back(launchVisualStudio(exe,project,root_));
#else
            throw Error("Open in Visual Studio is Windows-only. Open visualstudio/Pandata.bank.vcxproj on Windows; no alternative program was launched.");
#endif
        }
    } catch(const std::exception& e){report.error=e.what();}
    try {save(report);} catch(const std::exception& e) {
        if(!report.error.empty()) report.error+="; ";
        report.error+="Evidence save failed after the attempted action: "+std::string(e.what())+". Output below is not durably recorded.";
    }
    return report;
}
}
