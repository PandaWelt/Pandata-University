#include "Mission.h"
#include "ProgressStore.h"
#include <chrono>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>

namespace {
int checks = 0;
void check(bool condition,const std::string& name) {
    ++checks; if (!condition) throw std::runtime_error("FAIL: " + name);
}
void rejects(const std::function<void()>& action,const std::string& name) {
    bool failed = false;
    try { action(); } catch (const pu::Error&) { failed = true; }
    check(failed,name);
}
const std::string sample = R"(mission 1 "sample" "1" "Sample"
requires "pilot"
scope "read" "src/example.h"
step "one" "Explain" "ack" "First"
why "Purpose"
action "Read"
expect "Understand the next action"
more "Details"
syntax "term" "Definition"
professional "Tradeoff"
hint "Hint one"
end
step "two" "Predict" "response" "Second"
why "Reason"
action "Predict briefly"
expect "A short response"
hint "Observe"
end
finish
)";
pu::Mission mission(const std::string& text = sample) { std::istringstream in(text); return pu::loadMission(in); }
std::string changed(std::string source,const std::string& old,const std::string& next) {
    const auto p = source.find(old); if (p == std::string::npos) throw std::runtime_error("test setup missing token");
    source.replace(p,old.size(),next); return source;
}
void put(const std::filesystem::path& file,const std::string& text) {
    std::ofstream out(file,std::ios::binary); out << text; if (!out) throw std::runtime_error("test fixture write failed");
}
std::string get(const std::filesystem::path& file) { std::ifstream in(file,std::ios::binary); return pu::readText(in); }
}
int main(int argc,char** argv) {
    std::filesystem::path scratch;
    try {
        check(argc == 3,"tests require actual mission and pilot file arguments");
        auto m = mission();
        std::string crlf;
        for (char c : sample) { if (c == '\n') crlf += '\r'; crlf += c; }
        check(mission(crlf).fingerprint == m.fingerprint, "CRLF/LF checkout does not invalidate progress");
        rejects([]{mission(changed(sample,"\"Predict\" \"response\"","\"Predict\" \"ack\""));},"prediction cannot accidentally use acknowledgement gate");
        check(m.steps.size() == 2 && m.steps[0].syntax.size() == 1,"load typed steps and optional data");
        rejects([]{mission(changed(sample,"mission 1","mission 2"));},"reject unsupported version");
        rejects([]{mission(changed(sample,"\"Predict\"","\"Run\""));},"reject unimplemented executable type");
        rejects([]{mission(changed(sample,"\"response\"","\"always-pass\""));},"reject unknown gate");
        rejects([]{mission(changed(sample,"expect \"A short response\"\n",""));},"reject missing required field");
        rejects([]{mission(changed(sample,"step \"two\"","step \"one\""));},"reject duplicate step IDs");
        rejects([]{mission(changed(sample,"why \"Purpose\"","why \"Purpose\"\nwhy \"Again\""));},"reject duplicate fields");
        rejects([]{mission(changed(sample,"src/example.h","../outside.h"));},"reject traversal");
        rejects([]{mission(changed(sample,"finish\n",""));},"reject truncation");
        rejects([]{mission(sample+"finish\n");},"reject trailing records");
        rejects([]{mission(changed(sample,"\"Sample\"","Sample"));},"require quoted data");
        rejects([]{mission(std::string(1024*1024+1,'x'));},"bound input size");
        std::istringstream pilots("pilot 1\ncompleted \"pilot\" \"course/evidence/pilot.md\" \"reviewer-accepted\"\n");
        auto accepted = pu::loadPilot(pilots); pu::checkPrerequisites(m,accepted); ++checks;
        rejects([&]{pu::checkPrerequisites(m,{});},"reject missing prerequisites");
        std::istringstream duplicatePilot("pilot 1\ncompleted \"pilot\" \"course/evidence/a.md\" \"accepted\"\ncompleted \"pilot\" \"course/evidence/a.md\" \"accepted\"\n");
        rejects([&]{pu::loadPilot(duplicatePilot);},"reject duplicate imported pilot record");
        auto p = pu::freshProgress(m);
        check(pu::currentStep(p) == 0,"start on first step");
        rejects([&]{pu::recordAnswer(m,p,"wrong step");},"acknowledgement cannot be forged by an answer");
        check(pu::revealHint(m,p) == "Hint one" && p.steps[0].hintsUsed == 1,"record hint disclosure");
        rejects([&]{pu::revealHint(m,p);},"hint limit");
        pu::advance(m,p);
        rejects([&]{pu::advance(m,p);},"cannot bypass response gate");
        rejects([&]{pu::recordAnswer(m,p,"   ");},"blank answer rejected");
        rejects([&]{pu::recordAnswer(m,p,std::string(241,'x'));},"long answer rejected");
        pu::recordAnswer(m,p,"first attempt"); pu::recordAnswer(m,p,"quote \" and path \\ and panda");
        const auto bytes = pu::serializeProgress(m,p); std::istringstream roundtrip(bytes);
        auto decoded = pu::parseProgress(m,roundtrip);
        check(decoded.steps[1].attempts == p.steps[1].attempts && decoded.steps[0].hintsUsed == 1,"roundtrip responses/hints without loss");
        pu::advance(m,p); check(pu::currentStep(p) == 2,"deterministic prepared-step completion");
        auto invalid = pu::freshProgress(m); invalid.steps[1].complete = true;
        rejects([&]{pu::validateProgress(m,invalid);},"no completion out of order");
        auto altered = mission(changed(sample,"\"Purpose\"","\"Changed purpose\""));
        rejects([&]{pu::validateProgress(altered,p);},"content changes invalidate prior completion without migration");
        const auto unique = std::to_string(std::chrono::steady_clock::now().time_since_epoch().count());
        scratch = std::filesystem::temp_directory_path()/ ("pandata-university-tests-" + unique);
        check(std::filesystem::create_directory(scratch),"isolated test directory created");
        const auto directory = scratch/"progress";
        auto loaded = pu::loadProgress(directory,m), stale = loaded;
        check(!std::filesystem::exists(directory),"loading new progress does not write files");
        pu::saveProgress(directory,m,loaded);
        check(loaded.generation == 1,"first save");
        const auto first = get(directory/"1.pup");
        rejects([&]{pu::saveProgress(directory,m,stale);},"stale concurrent save rejected");
        pu::advance(m,loaded.value); pu::saveProgress(directory,m,loaded);
        auto resumed = pu::loadProgress(directory,m);
        check(resumed.generation == 2 && pu::currentStep(resumed.value)==1,"resume saved step");
        put(directory/"3.pup","truncated");
        auto recovered = pu::loadProgress(directory,m);
        check(recovered.generation==3 && pu::currentStep(recovered.value)==1 && !recovered.warnings.empty(),"recover previous valid snapshot with warning");
        pu::saveProgress(directory,m,recovered);
        check(recovered.generation==4 && get(directory/"3.pup")=="truncated" && get(directory/"1.pup")==first,"recovery preserves malformed and original snapshots");
        put(directory/"5.pup.tmp","interrupted write");
        auto interrupted = pu::loadProgress(directory,m);
        check(interrupted.generation==5 && !interrupted.warnings.empty(),"interrupted save recognized");
        pu::saveProgress(directory,m,interrupted);
        check(interrupted.generation==6 && get(directory/"5.pup.tmp")=="interrupted write","pending write is not overwritten");
        std::filesystem::create_directory(directory/".save-lock");
        rejects([&]{pu::saveProgress(directory,m,interrupted);},"active or stale lock blocks writes");
        std::filesystem::remove(directory/".save-lock");
        rejects([&]{pu::loadProgress(directory,altered);},"store refuses changed definition");
        std::filesystem::create_directory(scratch/"bad"); put(scratch/"bad/1.pup","bad");
        rejects([&]{pu::loadProgress(scratch/"bad",m);},"all-corrupt progress not silently reset");
        check(get(scratch/"bad/1.pup")=="bad","corrupt evidence remains intact");
        std::ifstream realMission(argv[1],std::ios::binary), realPilot(argv[2],std::ios::binary);
        check(bool(realMission)&&bool(realPilot),"real course fixtures readable");
        auto prepared = pu::loadMission(realMission); auto records = pu::loadPilot(realPilot); pu::checkPrerequisites(prepared,records);
        check(prepared.steps.size()==5 && records.size()==2,"actual U1 lesson and pilot compatibility load");
        std::filesystem::remove_all(scratch); // Only this test-created unique scratch directory.
        std::cout << "PASS: " << checks << " checks\n"; return 0;
    } catch (const std::exception& e) { std::cerr << e.what() << "\nTest scratch preserved: " << scratch.string() << '\n'; return 1; }
}
