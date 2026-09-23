#include "desktop/DesktopView.h"
#include "imgui.h"
#include <algorithm>
#include <cstdio>

namespace pu::desktop {
namespace {
void text(const std::string& s) { ImGui::TextWrapped("%s", s.c_str()); }
void heading(const char* s) { ImGui::Spacing(); ImGui::TextColored(ImVec4(.42f,.83f,.73f,1), "%s", s); ImGui::Separator(); }
void viewer(const char* id, const std::string& value, std::string& cache, std::vector<char>& buffer, float height) {
    if (cache != value) { cache = value; buffer.assign(value.begin(),value.end()); buffer.push_back(0); }
    ImGui::InputTextMultiline(id,buffer.data(),buffer.size(),ImVec2(-1,height),ImGuiInputTextFlags_ReadOnly);
}
}
void DesktopView::theme(float dpi) {
    ImGui::StyleColorsDark(); auto& s = ImGui::GetStyle();
    s.WindowRounding=0; s.ChildRounding=12; s.FrameRounding=6; s.GrabRounding=6;
    s.WindowPadding=ImVec2(22,18); s.FramePadding=ImVec2(12,8); s.ItemSpacing=ImVec2(10,10);
    s.Colors[ImGuiCol_WindowBg]=ImVec4(.055f,.072f,.09f,1);
    s.Colors[ImGuiCol_ChildBg]=ImVec4(.083f,.107f,.13f,1);
    s.Colors[ImGuiCol_FrameBg]=ImVec4(.12f,.16f,.19f,1);
    s.Colors[ImGuiCol_Button]=ImVec4(.13f,.30f,.29f,1);
    s.Colors[ImGuiCol_ButtonHovered]=ImVec4(.18f,.43f,.39f,1);
    s.Colors[ImGuiCol_ButtonActive]=ImVec4(.20f,.51f,.44f,1);
    s.Colors[ImGuiCol_Header]=ImVec4(.14f,.28f,.29f,1);
    s.Colors[ImGuiCol_Text]=ImVec4(.91f,.94f,.94f,1);
    s.Colors[ImGuiCol_TextDisabled]=ImVec4(.58f,.66f,.70f,1);
    s.Colors[ImGuiCol_PlotHistogram]=ImVec4(.36f,.78f,.64f,1);
    s.ScaleAllSizes(dpi); s.FontSizeBase=18; s.FontScaleDpi=dpi;
}
void DesktopView::overview() {
    heading("YOUR COURSE");
    text("Small steps. Real C++. Your PandaBank project.");
    text("Pick up where you left off, inspect the code, and record what you observe.");
    if (ImGui::Button("Continue learning",ImVec2(230,0))) {
        if(answer_[0]) draftError_="Return to Current lesson and save or clear your draft first.";
        else if(desk_.resume()) home_=false;
    }
    std::size_t requested=0;
    for (const auto& c : desk_.cards()) {
        ImGui::PushID(static_cast<int>(c.number));
        ImGui::Separator(); text(c.title);
        ImGui::Text("%s  |  %zu / %zu steps recorded",c.state.c_str(),c.completed,c.total);
        ImGui::ProgressBar(c.total ? static_cast<float>(c.completed)/static_cast<float>(c.total) : 0,ImVec2(-1,8),"");
        if (!c.detail.empty()) text(c.detail);
        ImGui::BeginDisabled(c.state=="blocked");
        if (ImGui::Button("Open lesson")) requested=c.number;
        ImGui::EndDisabled(); ImGui::PopID();
    }
    if(requested) {
        if(answer_[0]) draftError_="Return to Current lesson and save or clear your draft first.";
        else if(desk_.select(requested)) home_=false;
    }
    if(!draftError_.empty()) text(draftError_);
    heading("YOUR WORK STAYS YOURS");
    text("Answers and hint use are saved locally. A recorded step is not an automatic correctness or mastery grade.");
    if(ImGui::CollapsingHeader("Offline tools and limitations")) {
        text("Code viewing is read-only. Edit PandaBank in Visual Studio. University never applies a proposal or refactor. The console launcher remains available as a fallback.");
        text("Back up course/university/progress with matching course definitions; personal progress is not in Git.");
    }
}
void DesktopView::lesson() {
    const auto& selected=desk_.lesson();
    if (!selected) { text("No accessible lesson. Review dashboard diagnostics; saved progress was not reset."); return; }
    const auto& l=*selected; const auto i=currentStep(l.progress.value);
    const auto key=l.mission.id+":"+std::to_string(i);
    if(identity_!=key) { identity_=key; answer_.fill(0); draftError_.clear(); }
    heading("CURRENT MISSION"); text(l.mission.title);
    ImGui::Text("%zu / %zu steps recorded  |  snapshot %llu",i,l.mission.steps.size(),static_cast<unsigned long long>(l.progress.generation));
    ImGui::ProgressBar(static_cast<float>(i)/static_cast<float>(l.mission.steps.size()),ImVec2(-1,9),"");
    for(const auto& w:l.progress.warnings) text("Recovery: "+w);
    if(i==l.mission.steps.size()) {
        text("Prepared evidence collected. Your implementation still needs build evidence and review.");
        if(ImGui::Button("Browse lessons")) home_=true;
    } else {
        const auto& s=l.mission.steps[i]; ImGui::PushID(key.c_str());
        ImGui::TextColored(ImVec4(.96f,.72f,.40f,1),"STEP %zu OF %zu",i+1,l.mission.steps.size());
        text(s.title);
        if(ImGui::CollapsingHeader("Why this step")) text(s.why);
        heading("YOUR NEXT ACTION"); text(s.action);
        text("Here, Save answer records your response and Next step continues.");
        if(!l.progress.value.steps[i].attempts.empty()) {
            heading("LAST SAVED ANSWER"); text(l.progress.value.steps[i].attempts.back());
        }
        if(s.gate==Gate::Response) {
            heading("ONE SHORT OBSERVATION");
            ImGui::SetNextItemWidth(-1);
            const bool submit=ImGui::InputText("##answer",answer_.data(),answer_.size(),ImGuiInputTextFlags_EnterReturnsTrue);
            text("One short line, up to 240 bytes. Press Enter or Save answer to save.");
            if(ImGui::Button("Save answer") || submit) { if(desk_.answer(answer_.data())) { answer_.fill(0); draftError_.clear(); } }
            ImGui::SameLine();
        }
        if(ImGui::Button("Next step")) {
            if(answer_[0]) draftError_="Save or clear your draft before moving on.";
            else { desk_.next(); draftError_.clear(); }
        }
        ImGui::SameLine();
        ImGui::BeginDisabled(l.progress.value.steps[i].hintsUsed>=s.hints.size());
        if(ImGui::Button("Prepared hint")) desk_.hint();
        ImGui::EndDisabled();
        if(!draftError_.empty()) text(draftError_);
        if(ImGui::CollapsingHeader("Expected result")) text(s.expected);
        if(ImGui::CollapsingHeader("Explain more")) text(s.more.empty()?"No extra explanation is needed for this step.":s.more);
        if(ImGui::CollapsingHeader("Syntax / vocabulary")) {
            if(s.syntax.empty()) text("No new syntax for this step.");
            for(const auto& [term,meaning]:s.syntax) { ImGui::BulletText("%s",term.c_str()); text(meaning); }
        }
        if(ImGui::CollapsingHeader("Professional alternatives")) text(s.professional.empty()?"Keep this step focused; no extra alternative is prescribed.":s.professional);
        ImGui::PopID();
    }
    if(ImGui::CollapsingHeader("Saved history / review report")) viewer("##report",desk_.report(),reportCache_,reportBuffer_,260);
    if(ImGui::CollapsingHeader("Compiler, linker and save troubleshooting")) text(StudyDesk::feedback());
}
void DesktopView::workspace() {
    heading("PROJECT WORKSPACE");
    if(ImGui::BeginTabBar("workspace")) {
        if(ImGui::BeginTabItem("Code / files")) {
            text("Read-only relevant files. Select text and Ctrl+C to copy. No file-save or apply command.");
            const auto chosen=desk_.selectedFile();
            const char* label=chosen ? desk_.files()[chosen-1].c_str() : "Choose a reviewed file";
            ImGui::SetNextItemWidth(-1);
            if(ImGui::BeginCombo("##files",label)) {
                for(std::size_t n=0;n<desk_.files().size();++n) if(ImGui::Selectable(desk_.files()[n].c_str(),chosen==n+1)) desk_.view(n+1);
                ImGui::EndCombo();
            }
            if(ImGui::Button("Reload file") && desk_.selectedFile()) desk_.view(desk_.selectedFile());
            viewer("##code",desk_.fileText(),fileCache_,fileBuffer_,std::max(120.0f,ImGui::GetContentRegionAvail().y-4));
            ImGui::EndTabItem();
        }
        if(ImGui::BeginTabItem("Checks / output")) {
            if(ImGui::Button("Check course")) desk_.checkCourse();
            ImGui::BeginDisabled(desk_.busy());
            if(ImGui::Button("Run University checks")) desk_.start(Action::UniversityCheck);
            if(ImGui::Button("Git status")) desk_.start(Action::GitStatus);
            ImGui::SameLine(); if(ImGui::Button("Git diff")) desk_.start(Action::GitDiff);
            if(ImGui::Button("Open PandaBank in Visual Studio")) desk_.start(Action::OpenVisualStudio);
            ImGui::EndDisabled();
            text("University checks test the course engine. Build your PandaBank changes in Visual Studio.");
            viewer("##output",desk_.output(),outputCache_,outputBuffer_,std::max(120.0f,ImGui::GetContentRegionAvail().y-4));
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
    }
}
void DesktopView::draw() {
    desk_.poll();
    const auto* vp=ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(vp->WorkPos); ImGui::SetNextWindowSize(vp->WorkSize);
    ImGui::Begin("Pandata University",nullptr,ImGuiWindowFlags_NoDecoration|ImGuiWindowFlags_NoMove|ImGuiWindowFlags_NoSavedSettings);
    ImGui::TextColored(ImVec4(.42f,.83f,.73f,1),"PU / PANDATA UNIVERSITY");
    ImGui::SameLine(); ImGui::TextDisabled("Desktop 1  |  Offline study");
    if(ImGui::Button("Home")) home_=true;
    ImGui::SameLine(); if(ImGui::Button("Current lesson")) showCurrentLesson();
    ImGui::SameLine(); ImGui::SetNextItemWidth(120);
    if(ImGui::SliderFloat("Text size",&scale_,.85f,1.5f,"%.2fx")) ImGui::GetStyle().FontScaleMain=scale_;
    ImGui::BeginDisabled(); ImGui::Button("AI mentor unavailable"); ImGui::EndDisabled();
    ImGui::SameLine(); text(DeskController::mentorState);
    ImGui::Separator();
    const auto available=ImGui::GetContentRegionAvail(); const bool wide=available.x>960*ImGui::GetStyle().FontScaleDpi;
    const float bodyHeight=std::max(200.0f,available.y-95);
    if(wide) {
        ImGui::BeginChild("lesson",ImVec2(available.x*.54f,bodyHeight),ImGuiChildFlags_Borders);
        if(home_) overview(); else lesson(); ImGui::EndChild(); ImGui::SameLine();
        ImGui::BeginChild("project",ImVec2(0,bodyHeight),ImGuiChildFlags_Borders); workspace(); ImGui::EndChild();
    } else if(ImGui::BeginTabBar("compact")) {
        if(ImGui::BeginTabItem("Learn")) { ImGui::BeginChild("lesson",ImVec2(0,bodyHeight-40)); if(home_) overview(); else lesson(); ImGui::EndChild(); ImGui::EndTabItem(); }
        if(ImGui::BeginTabItem("Workspace")) { ImGui::BeginChild("project",ImVec2(0,bodyHeight-40)); workspace(); ImGui::EndChild(); ImGui::EndTabItem(); }
        ImGui::EndTabBar();
    }
    ImGui::Separator(); text(desk_.message());
    if(desk_.busy()) text("Action running. Closing waits for the bounded child process; keep this window open for the result.");
    ImGui::End();
}
}
