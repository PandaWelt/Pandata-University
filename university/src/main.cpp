#include "Mission.h"
#include "ProgressStore.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

namespace {
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
                     "Your responses are saved; the next engineering stage is U2 repository actions.\n";
        return;
    }
    const auto& s = m.steps[i];
    std::cout << "Step " << i+1 << '/' << m.steps.size() << ": " << s.title << "\n\n"
              << s.why << "\n\nAction: " << s.action << "\nExpected: " << s.expected << '\n';
    if (!p.steps[i].attempts.empty()) std::cout << "Saved response: " << p.steps[i].attempts.back() << '\n';
    std::cout << "\nCommands: next | answer <short response> | more | syntax | professional | hint | check | quit\n"
                 "Mentor, Rescue and Maintenance are unavailable in U1.\n";
}
}
int main(int argc,char** argv) {
    try {
        bool validate = false;
        std::filesystem::path missionFile = "course/university/missions/u1-boundaries.pum";
        if (argc >= 2 && std::string(argv[1]) == "--validate") validate = true;
        else if (argc >= 2) missionFile = argv[1];
        if (argc > 2) throw pu::Error("Usage: pandata-university [--validate | mission-file]");
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
        display(mission,loaded.value);
        std::string command;
        while (pu::currentStep(loaded.value) < mission.steps.size() && std::cout << "> " && std::getline(std::cin,command)) {
            if (command == "quit") break;
            const auto i = pu::currentStep(loaded.value); const auto& step = mission.steps[i];
            try {
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
            } catch (const std::exception& error) { std::cout << "Not advanced: " << error.what() << '\n'; }
        }
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Pandata University stopped safely: " << error.what() << "\nExisting mission and progress files were not reset.\n";
        return 1;
    }
}
