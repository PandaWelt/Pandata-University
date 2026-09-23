#include "StudyDesk.h"
#include "RepositoryActions.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
namespace fs=std::filesystem;
namespace {
int checks=0;
void check(bool yes,const char* why) {if(!yes)throw pu::Error(why);++checks;}
template<class F> void refuse(F f,const char* why) {bool rejected=false;try{f();}catch(const std::exception&){rejected=true;}check(rejected,why);}
void put(const fs::path& p,const std::string& data) {
    fs::create_directories(p.parent_path());std::ofstream out(p,std::ios::binary);out<<data;out.close();
    if(!out)throw pu::Error("test write failed");
}
std::string get(const fs::path& p) {std::ifstream in(p,std::ios::binary);if(!in)throw pu::Error("test input missing");return pu::readText(in);}
void setup(const fs::path& source,const fs::path& root) {
    if(!fs::create_directory(root))throw pu::Error("Test directory exists; nothing replaced");
    for(auto file:{"course/university/missions/u1-boundaries.pum","course/university/missions/003-object-types.pum","course/university/pilot.puc","university/include/Mission.h","src/main.cpp","src/UI/AppUISetup.cpp"})
        put(root/file,get(source/file));
}
void finish(pu::StudyDesk& desk,pu::StudyLesson& l) {
    while(pu::currentStep(l.progress.value)<l.mission.steps.size()) {
        auto p=l.progress;const auto i=pu::currentStep(p.value);
        if(l.mission.steps[i].gate==pu::Gate::Response)pu::recordAnswer(l.mission,p.value,"SYNTHETIC TEST EVIDENCE - NOT CHRIS");
        pu::advance(l.mission,p.value);desk.save(l,std::move(p));
    }
}
void prepare(const fs::path& source,const fs::path& stage) {
    const auto root=stage/"workflow";setup(source,root);pu::StudyDesk desk(root);auto first=desk.open(1);finish(desk,first);
    put(stage/"u1-before.txt",get(root/"course/university/progress/u1-boundaries/5.pup"));
    put(stage/"study-first.txt","home\r\nlesson 2\r\nnext\r\nanswer synthetic test prediction\r\nhint\r\nhistory\r\nstep 1\r\nstep 7\r\nreport\r\nfeedback\r\nfiles\r\nview 1\r\nview 10\r\nlesson ../x\r\nreview-select\r\nreview-preview\r\nreview-apply\r\nreview-dismiss\r\nquit\r\n");
    put(stage/"study-resume.txt","resume\r\nhistory\r\nnext\r\nanswer synthetic dependency observation\r\nnext\r\nreport\r\nquit\r\n");
    std::cout<<"Prepared disposable workflow; no learner progress was accessed.\n";
}
void verify(const fs::path& stage) {
    auto root=stage/"workflow";pu::StudyDesk desk(root);auto second=desk.open(2);
    check(pu::currentStep(second.progress.value)==3,"relaunch resumed and advanced real course controller");
    check(second.progress.value.steps[1].attempts.back()=="synthetic test prediction","first process answer retained");
    check(second.progress.value.steps[1].hintsUsed==1,"hint retained across process restart");
    check(second.progress.value.steps[2].attempts.back()=="synthetic dependency observation","second process answer retained");
    check(get(root/"course/university/progress/u1-boundaries/5.pup")==get(stage/"u1-before.txt"),"U1 snapshot unchanged");
    check(!fs::exists(root/"course/university/evidence"),"study/review created no machine evidence");
    check(!fs::exists(root/"src/Objects"),"lesson did not implement the extraction");
    for(auto marker:{"STUDY REPORT", "PREPARED OFFLINE TROUBLESHOOTING", "READ-ONLY STEP REVIEW", "DISPLAYED: U4-F1", "REJECTED: use exactly", "Action/step not completed:", "src/main.cpp (read-only)"})
        check(get(stage/"study-first-output.txt").find(marker)!=std::string::npos,"actual console first-session routing");
    check(get(stage/"study-resume-output.txt").find("synthetic test prediction")!=std::string::npos,"actual resumed console displays saved response");
    std::cout<<"PASS: "<<checks<<" offline console integration checks\n";
}
void tests(const fs::path& source,const fs::path& root) {
    setup(source,root);pu::StudyDesk desk(root);desk.validateDefinitions();
    check(desk.catalog().size()==2,"fixed reviewed catalog");
    check(desk.catalog()[0].state=="ready","orientation ready without fabricated completion");
    check(desk.catalog()[1].state=="blocked","real next lesson locked until saved orientation");
    refuse([&]{desk.open(2);},"cannot bypass prerequisite with lesson number");
    for(auto bad:{"", "0", "01", "+1", "-1", "1 ", " 1", "1\t", "1\r", "1\n", "1 x", "1&cmd", "1|cmd", "../1", "C:\\1", "1;cmd", "1$(cmd)", "ONE", "99999999999999999999999999999999"})
        refuse([&]{pu::studyNumber(bad);},"malformed number refused");
    refuse([&]{pu::studyNumber(std::string("1\0x",3));},"NUL selection refused");
    check(pu::studyNumber("2")==2,"canonical lesson selection");
    refuse([&]{desk.open(0);},"zero outside catalog");refuse([&]{desk.open(3);},"unknown lesson outside catalog");
    auto first=desk.open(1);
    check(!desk.home().empty() && !pu::StudyDesk::report(first).empty(),"home/report useful without writes");
    check(!fs::exists(root/"course/university/progress"),"read-only browsing creates no progress");
    const auto pilot=get(root/"course/university/pilot.puc");
    put(root/"course/university/pilot.puc",pilot+"completed \"u1-boundaries\" \"fake\" \"fake\"\n");
    refuse([&]{desk.open(2);},"pilot claim cannot bypass registered snapshot prerequisite");put(root/"course/university/pilot.puc",pilot);
    finish(desk,first);const auto u1=get(root/"course/university/progress/u1-boundaries/5.pup");
    check(desk.catalog()[0].state=="recorded" && desk.catalog()[1].state=="ready","completed snapshots unlock next lesson");
    auto l=desk.resume();check(l.number==2,"resume recommends real next lesson");
    check(pu::StudyDesk::reviewStep(l,1).find("Resume position unchanged")!=std::string::npos,"review active step read-only");
    refuse([&]{pu::StudyDesk::reviewStep(l,2);},"future step not revealed through review");
    auto p=l.progress;pu::advance(l.mission,p.value);desk.save(l,p);
    refuse([&]{auto c=l.progress;pu::advance(l.mission,c.value);},"response gate retained");
    p=l.progress;pu::recordAnswer(l.mission,p.value,"My prediction");desk.save(l,p);
    p=l.progress;pu::revealHint(l.mission,p.value);desk.save(l,p);
    auto resumed=pu::StudyDesk(root).resume();
    check(resumed.number==2 && pu::currentStep(resumed.progress.value)==1,"cross-process-style resume keeps position");
    check(resumed.progress.value.steps[1].attempts.back()=="My prediction","answer preserved");
    check(resumed.progress.value.steps[1].hintsUsed==1,"hint count preserved");
    check(pu::StudyDesk::history(resumed).find("Learner-reported: My prediction")!=std::string::npos,"origin in history");
    check(pu::StudyDesk::report(resumed).find("No compiler pass")!=std::string::npos,"report cannot grant build/mastery");
    check(desk.home().find("Deferred:")!=std::string::npos,"blocked capabilities visible");
    const auto before=resumed.progress.generation;
    auto stale=desk.open(2);p=l.progress;pu::recordAnswer(l.mission,p.value,"Updated prediction");desk.save(l,p);
    refuse([&]{desk.save(stale,stale.progress);},"stale session cannot overwrite another save");
    check(stale.progress.generation==before,"failed save leaves memory unchanged");
    const auto definition=get(root/"course/university/missions/003-object-types.pum");
    put(root/"course/university/missions/003-object-types.pum",definition+"\n# changed wording\n");
    refuse([&]{desk.save(l,l.progress);},"changed fingerprint stops current-session save");
    refuse([&]{desk.open(2);},"changed definition cannot reuse completion");
    put(root/"course/university/missions/003-object-types.pum",definition);
    put(root/"course/university/progress/u1-boundaries/6.pup","corrupt\n");
    refuse([&]{desk.open(2);},"recovered prerequisite needs explicit review");
    check(!desk.open(1).progress.warnings.empty(),"recovery diagnostic retained");
    fs::remove(root/"course/university/progress/u1-boundaries/6.pup");
    pu::RepositoryActions actions(root);
    check(actions.files().size()==11 && actions.files()[9]=="src/Objects/ObjectTypes.h" && actions.files()[10]=="src/Objects/ObjectTypes.cpp","only reviewed new relevant files added");
    refuse([&]{actions.view(12);},"out of scope file number refused");
    refuse([&]{actions.view(10);},"missing learner file reports useful error");
    put(root/"src/Objects/ObjectTypes.h","// learner test file\n");
    check(actions.view(10).find("learner test file")!=std::string::npos,"read-only learner file view");
    put(root/"outside.txt","unrelated\n");
    fs::remove(root/"src/Objects/ObjectTypes.h");std::error_code ec;
    fs::create_symlink(root/"outside.txt",root/"src/Objects/ObjectTypes.h",ec);
    if(ec)std::cout<<"SKIP: symlink creation denied; not passed.\n";
    else {refuse([&]{actions.view(10);},"learner file symlink refused");fs::remove(root/"src/Objects/ObjectTypes.h");}
    l=desk.open(2);finish(desk,l);
    check(desk.catalog()[1].state=="recorded","lesson records evidence without acceptance promotion");
    check(desk.resume().number==2,"completed catalog resumes latest lesson for report");
    check(get(root/"course/university/progress/u1-boundaries/5.pup")==u1,"orientation bytes unchanged by later lesson");
    check(!fs::exists(root/"course/university/evidence"),"workspace never manufactures machine evidence");
    check(pu::StudyDesk::feedback().find("not your PandaBank refactor")!=std::string::npos,"prepared help distinguishes tests from product");
    put(root/"course/university/missions/003-object-types.pum","bad\n");
    check(desk.catalog()[1].state=="blocked","invalid definition isolated and diagnosed");
    refuse([&]{desk.validateDefinitions();},"catalog check detects invalid definition");
    std::cout<<"PASS: "<<checks<<" offline study checks\n";
}
}
int main(int argc,char** argv) {
    fs::path scratch;
    try {
        if(argc==4 && std::string(argv[1])=="--prepare") {prepare(fs::absolute(argv[2]),fs::absolute(argv[3]));return 0;}
        if(argc==3 && std::string(argv[1])=="--verify") {verify(fs::absolute(argv[2]));return 0;}
        if(argc!=2)throw pu::Error("Use repository, --prepare repository staging, or --verify staging");
        scratch=fs::temp_directory_path()/("pu-offline-study-"+std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
        tests(fs::absolute(argv[1]),scratch);fs::remove_all(scratch);return 0;
    }catch(const std::exception& e){std::cerr<<"FAIL: "<<e.what()<<"\nScratch preserved: "<<scratch<<'\n';return 1;}
}
