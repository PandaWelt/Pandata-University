#include "desktop/DesktopView.h"
#include "imgui.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
namespace fs=std::filesystem;
namespace {
int checks=0;
void check(bool value,const char* name) { if(!value) throw pu::Error(name); ++checks; }
void put(const fs::path& path,const std::string& s) { fs::create_directories(path.parent_path()); std::ofstream f(path,std::ios::binary); f<<s; f.close(); if(!f) throw pu::Error("test write failed"); }
std::string get(const fs::path& path) { std::ifstream f(path,std::ios::binary); if(!f) throw pu::Error("test read failed"); return pu::readText(f); }
void frames(pu::desktop::DeskController& d) {
    ImGui::CreateContext(); auto& io=ImGui::GetIO(); io.IniFilename=nullptr; io.LogFilename=nullptr;
    pu::desktop::DesktopView::theme(1);
    unsigned char* pixels=nullptr; int w=0,h=0; io.Fonts->GetTexDataAsRGBA32(&pixels,&w,&h);
    io.Fonts->SetTexID(static_cast<ImTextureID>(1));
    pu::desktop::DesktopView view(d);
    for(int page=0;page<2;++page) {
      if(page) view.showCurrentLesson();
      for(const auto size:{ImVec2(1440,900),ImVec2(880,620),ImVec2(2200,1400)}) {
        for(int n=0;n<3;++n) {
            io.DisplaySize=size; io.DeltaTime=1.0f/60; ImGui::NewFrame(); view.draw(); ImGui::Render();
        }
        check(ImGui::GetDrawData()->TotalVtxCount>0,"real desktop view renders at wide/compact/large size");
      }
    }
    ImGui::DestroyContext();
}
}
int main(int argc,char** argv) {
    try {
        if(argc!=3) throw pu::Error("Use source-repository fresh-disposable-directory");
        auto source=fs::absolute(argv[1]), root=fs::absolute(argv[2]);
        if(fs::exists(root)) throw pu::Error("Disposable directory already exists; nothing replaced");
        fs::create_directories(root);
        for(auto p:{"course/university/missions/u1-boundaries.pum","course/university/missions/003-object-types.pum","course/university/pilot.puc","src/main.cpp","src/UI/AppUISetup.cpp","university/include/Mission.h"}) put(root/p,get(source/p));
        using pu::desktop::DeskController;
        DeskController d(root);
        check(d.cards().size()==2,"fixed catalog exposed");
        check(d.lesson()->number==1,"fresh orientation selected");
        check(!d.select(2),"prerequisite cannot be bypassed from UI");
        check(!d.select(0) && !d.select(999),"invalid lessons refused");
        check(!fs::exists(root/"course/university/progress"),"home/open refusals do not save");
        check(d.checkCourse() && d.output().find("PASS:")==0,"course check gives real output");
        frames(d);
        check(!fs::exists(root/"course/university/progress"),"view frames do not save");
        check(d.next(),"acknowledgment saved");
        check(!d.next(),"response gate enforced");
        check(!d.answer("") && !d.answer(std::string(241,'x')) && !d.answer("line\nbreak"),"invalid answer refused");
        check(d.answer("SYNTHETIC desktop prediction"),"short answer saved");
        check(d.hint(),"prepared hint saved");
        DeskController restarted(root);
        check(restarted.lesson()->progress.value.steps[1].attempts.back()=="SYNTHETIC desktop prediction","desktop restart restores answer");
        check(restarted.lesson()->progress.value.steps[1].hintsUsed==1,"desktop restart restores hint");
        auto stale=restarted.lesson()->progress.generation;
        check(d.answer("SYNTHETIC newer observation"),"new snapshot saved");
        check(!restarted.next() && restarted.lesson()->progress.generation==stale,"stale desktop refuses without advancing memory");
        while(pu::currentStep(d.lesson()->progress.value)<d.lesson()->mission.steps.size()) {
            auto i=pu::currentStep(d.lesson()->progress.value);
            if(d.lesson()->mission.steps[i].gate==pu::Gate::Response) check(d.answer("SYNTHETIC orientation only"),"synthetic response");
            check(d.next(),"synthetic advance");
        }
        check(d.resume() && d.lesson()->number==2,"completed U1 unlocks Mission003");
        check(d.next() && !d.next(),"Mission003 short-step gate retained");
        check(d.answer("SYNTHETIC Mission003 prediction"),"Mission003 save");
        DeskController again(root);
        check(again.lesson()->number==2 && pu::currentStep(again.lesson()->progress.value)==1,"resume Mission003 actual store");
        check(d.view(1) && !d.fileText().empty(),"allowlisted source displayed");
        check(!d.view(999) && d.fileText().empty() && d.selectedFile()==0,"refused replacement clears stale source");
        check(!d.view(10),"missing learner file has useful refusal");
        check(!d.start(static_cast<pu::desktop::Action>(999)) && !d.busy(),"invalid action cannot dispatch");
        check(d.report().find("No compiler pass")!=std::string::npos,"report cannot grant mastery");
        check(d.history().find("SYNTHETIC Mission003 prediction")!=std::string::npos,"history carries observations");
        check(std::string(DeskController::mentorState).find("No context is sent")!=std::string::npos,"mentor truthfully offline");
        const auto saved=d.lesson()->progress.generation;
        check(d.start(pu::desktop::Action::GitStatus),"fixed async action starts");
        check(!d.start(pu::desktop::Action::GitDiff),"overlapping job refused");
        const auto deadline=std::chrono::steady_clock::now()+std::chrono::seconds(40);
        while(d.busy() && std::chrono::steady_clock::now()<deadline) { d.poll(); std::this_thread::sleep_for(std::chrono::milliseconds(10)); }
        check(!d.busy() && d.output().find("FAILED:")==0,"unversioned disposable root produces useful action failure");
        check(d.lesson()->progress.generation==saved,"action result does not save learner progress");
        check(fs::exists(root/"course/university/evidence/u2"),"action machine evidence captured separately");
        const auto definition=root/"course/university/missions/003-object-types.pum";
        put(definition,get(definition)+"\n# synthetic changed definition\n");
        check(!d.answer("must not save") && d.lesson()->progress.generation==saved,"definition mismatch preserves progress");
        check(!fs::exists(root/"src/Objects"),"no learner extraction implemented");
        std::cout<<"PASS: "<<checks<<" desktop controller/render checks; disposable state only; no U1-U4 suite rerun.\n";
        return 0;
    } catch(const std::exception& e) { std::cerr<<"FAIL: "<<e.what()<<'\n'; return 1; }
}
