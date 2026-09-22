#include "Mission.h"
#include "ProgressStore.h"
#include "RepositoryActions.h"
#include "mentor/Mentor.h"
#include "ProposalPreview.h"
#include "ProposalParser.h"
#include <charconv>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
void actionsHelp() {
    std::cout << "\nU2: files | view <number> | run-check | failure-demo | git-status | git-diff | open-vs | help | quit\n"
                 "run-check compiles and runs University engine tests; failure-demo uses a temporary invalid fixture.\n"
                 "Actions save command/output/exit evidence. No general shell or source editor is available.\n"
                 "U3: mentor-status | mentor-preview | mentor-send (after preview)\n"
                 "Mentor is optional, advisory and read-only. No progress is changed by these commands.\n"
                 "U4-F1: proposal-preview | proposal-stale-demo (in-memory fixture; nothing applied or saved)\n";
}
void showReport(const pu::ActionReport& r) {
    for (const auto& c:r.commands) {
        std::cout << "\nCommand: " << pu::terminalText(c.executable.string());
        for(const auto& a:c.arguments) std::cout << ' ' << pu::jsonString(a);
        std::cout << "\nWorking directory: " << pu::terminalText(c.cwd.string()) << '\n' << pu::terminalText(c.output);
        if(c.handedOff) std::cout << "Handoff recorded; IDE completion/exit not observed.\n";
        else std::cout << "Exit: " << c.exitCode << " | timeout: " << c.timedOut << " | output limit: " << c.outputLimited << '\n';
        if(!c.error.empty()) std::cout << pu::terminalText(c.error) << '\n';
    }
    if(!r.error.empty())std::cout << pu::terminalText(r.error) << '\n';
    std::cout << (r.ok()?"Action succeeded":"Action failed") << ". Evidence: " << pu::terminalText(r.evidence.string()) << '\n';
    if(r.action=="failure-demo")std::cout << "The deliberately invalid scratch mission should fail. This is not a normal check pass; original mission/progress are unchanged.\n";
}

std::ifstream open(const std::filesystem::path& file) {
    std::ifstream stream(file,std::ios::binary);
    if (!stream) throw pu::Error("Cannot read " + file.string() + ". Launch from the PandaBank repository root.");
    return stream;
}
void display(const pu::Mission& m,const pu::Progress& p) {
    const auto i = pu::currentStep(p);
    std::cout << "\nPandata University | Offline foundation\n" << m.title << '\n';
    if (i == m.steps.size()) {
        std::cout << "Prepared orientation steps recorded. No PandaBank mission or concept was automatically marked mastered.\n"
                     "Your responses are saved. U2 repository actions are now available; type run-check to begin.\n";
        return;
    }
    const auto& s = m.steps[i];
    std::cout << "Step " << i+1 << '/' << m.steps.size() << ": " << s.title << "\n\n"
              << s.why << "\n\nAction: " << s.action << "\nExpected: " << s.expected << '\n';
    if (!p.steps[i].attempts.empty()) std::cout << "Saved response: " << p.steps[i].attempts.back() << '\n';
    std::cout << "\nCommands: next | answer <short response> | more | syntax | professional | hint | check | quit\n"
                 "Use mentor-preview for an optional explanation. Rescue and Maintenance remain unavailable.\n";
}
}
int main(int argc,char** argv) {
    try {
        if (argc >= 2 && std::string(argv[1]).starts_with("--proposal-json")) {
            // Refuse extra payload/path arguments before any repository/progress access.
            const std::string flag = argv[1];
            if (argc != 2 || (flag != "--proposal-json-fixture" &&
                flag != "--proposal-json-malformed" && flag != "--proposal-json-stale")) {
                std::cout << "REJECTED: use one fixed serialized-fixture flag without arguments.\n";
                return 1;
            }
            const auto example = flag == "--proposal-json-malformed" ? pu::preview::SerializedFixture::malformed :
                flag == "--proposal-json-stale" ? pu::preview::SerializedFixture::stale : pu::preview::SerializedFixture::valid;
            const auto report = pu::preview::serializedFixturePreview(example);
            std::cout << report.text;
            return report.state == pu::preview::State::displayed ? 0 : 1;
        }
        if (argc == 2 && (std::string(argv[1]) == "--proposal-fixture" || std::string(argv[1]) == "--proposal-stale-fixture")) {
            const auto report = pu::preview::fixturePreview(std::string(argv[1]) == "--proposal-stale-fixture");
            std::cout << report.text;
            return report.state == pu::preview::State::displayed ? 0 : 1;
        }
        if(argc==3 && std::string(argv[1])=="--action") {
            pu::RepositoryActions actions(std::filesystem::current_path());
            auto result=actions.execute(argv[2]);showReport(result);return result.ok()?0:1;
        }
        bool validate = false;
        std::filesystem::path missionFile = "course/university/missions/u1-boundaries.pum";
        if (argc >= 2 && std::string(argv[1]) == "--validate") validate = true;
        else if (argc >= 2) missionFile = argv[1];
        if (argc > 2) throw pu::Error("Usage: pandata-university [--validate | mission-file | --action ACTION]");
        auto input = open(missionFile); const auto mission = pu::loadMission(input);
        auto pilotInput = open("course/university/pilot.puc");
        const auto pilot = pu::loadPilot(pilotInput); pu::checkPrerequisites(mission,pilot);
        if (validate) {
            std::cout << "Mission/schema and reviewed pilot prerequisites valid. No progress written.\n";
            return 0;
        }
        const auto progressDirectory = std::filesystem::path("course/university/progress") / mission.id;
        auto loaded = pu::loadProgress(progressDirectory,mission);
        for (const auto& warning : loaded.warnings) std::cout << "Recovery: " << warning << '\n';
        pu::RepositoryActions actions(std::filesystem::current_path());
        display(mission,loaded.value);actionsHelp();
        std::string command, mentorPreview;
        while (std::cout << "> " && std::getline(std::cin,command)) {
            if (command == "quit") break;
            try {
                if(command=="proposal-preview"||command=="proposal-stale-demo") {
                    std::cout << pu::preview::fixturePreview(command=="proposal-stale-demo").text;
                    continue;
                }
                if(command=="mentor-preview") {
                    mentorPreview=pu::mentor::context(mission,loaded.value);
                    std::cout << "Exact context for the fixed mentor question (no files/diffs attached):\n"
                              << pu::terminalText(mentorPreview) << "\nType mentor-send to send this bundle using your included ChatGPT allowance, or continue offline.\n";
                    continue;
                }
                if(command=="mentor-status"||command=="mentor-send") {
                    const auto bundle=pu::mentor::context(mission,loaded.value);
                    const bool send=command=="mentor-send";
                    if(send&&(mentorPreview.empty()||mentorPreview!=bundle))throw pu::Error("Use mentor-preview first. If the step/response changed, review the new context before sending.");
                    if(send)mentorPreview.clear();
                    std::cout << "Checking installed Codex and the included-only read-only boundary (bounded wait)...\n" << std::flush;
                    const auto result=pu::mentor::run(std::filesystem::current_path(),bundle,send);
                    std::cout << "Mentor: " << pu::terminalText(result.state) << " | plan: " << pu::terminalText(result.plan)
                              << " | model: " << pu::terminalText(result.model) << '\n' << pu::terminalText(result.detail) << '\n';
                    if(!result.answer.empty())std::cout << "ADVISORY ONLY - compiler/tests remain authoritative:\n" << pu::terminalText(result.answer);
                    const auto evidence=pu::mentor::record(std::filesystem::current_path(),send?bundle:"",result);
                    std::cout << "U3 evidence: " << pu::terminalText(evidence.string()) << "\nUniversity remains offline-capable. No source, Git, or progress was changed by this interaction.\n";
                    continue;
                }
                if(command=="help") {actionsHelp();continue;}
                if(command=="files") {
                    for(std::size_t n=0;n<actions.files().size();++n)std::cout << n+1 << " | " << actions.files()[n] << '\n';
                    continue;
                }
                if(command.starts_with("view ")) {
                    const auto value=command.substr(5);std::size_t n=0;
                    const auto [end,error]=std::from_chars(value.data(),value.data()+value.size(),n);
                    if(error!=std::errc{} || end!=value.data()+value.size())throw pu::Error("Use view followed by a file number from files, not a path or command.");
                    std::cout << actions.view(n);continue;
                }
                if(command=="run-check"||command=="failure-demo"||command=="git-status"||command=="git-diff"||command=="open-vs") {
                    showReport(actions.execute(command));continue;
                }
                const auto i = pu::currentStep(loaded.value);
                if(i==mission.steps.size()) {std::cout << "Orientation is already complete. Use a U2 action or quit.\n";continue;}
                const auto& step=mission.steps[i];
                auto candidate = loaded; std::string feedback; bool changed = false;
                if (command == "next") { pu::advance(mission,candidate.value); changed = true; }
                else if (command.starts_with("answer ")) {
                    pu::recordAnswer(mission,candidate.value,command.substr(7));
                    feedback = "Response saved, not automatically graded as correct. Use next when ready."; changed = true;
                } else if (command == "hint") { feedback = pu::revealHint(mission,candidate.value); changed = true; }
                else if (command == "more") feedback = step.more.empty() ? "No extra explanation for this step." : step.more;
                else if (command == "professional") feedback = step.professional.empty() ? "No professional comparison for this step." : step.professional;
                else if (command == "syntax") {
                    for (const auto& [term,meaning] : step.syntax) std::cout << term << ": " << meaning << '\n';
                    if (step.syntax.empty()) feedback = "No syntax notes for this step.";
                } else if (command == "check") {
                    pu::validateProgress(mission,loaded.value);
                    feedback = step.gate == pu::Gate::Response && loaded.value.steps[i].attempts.empty()
                        ? "Response missing: enter one short answer."
                        : "Step evidence gate satisfied. This checks record completeness, not C++ correctness or mastery.";
                } else feedback = "Unknown command. Use next, answer <text>, more, syntax, professional, hint, check or quit.";
                if (changed) {
                    // Save first. On failure keep the current in-memory step unchanged.
                    pu::saveProgress(progressDirectory,mission,candidate); loaded = std::move(candidate);
                    std::cout << "Saved snapshot " << loaded.generation << ".\n";
                }
                if (!feedback.empty()) std::cout << feedback << '\n';
                if (command == "next" && changed) display(mission,loaded.value);
            } catch (const std::exception& error) { std::cout << "Action/step not completed: " << error.what() << '\n'; }
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Pandata University stopped safely: " << error.what() << "\nExisting mission and progress files were not reset.\n";
        return 1;
    }
}
