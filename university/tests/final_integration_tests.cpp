#include "StudyDesk.h"
#include <iostream>
#include <string>

// Read-only verification of snapshots written by the real console, not a fake store.
int main(int argc, char** argv) {
    try {
        if (argc != 3) throw pu::Error("Use --first/--complete/--recovered/--blocked disposable-root");
        const std::string mode = argv[1];
        if (mode != "--first" && mode != "--complete" && mode != "--recovered" && mode != "--blocked")
            throw pu::Error("Unknown verification mode");
        pu::StudyDesk desk(argv[2]);
        const auto first = desk.open(1);
        if (pu::currentStep(first.progress.value) != first.mission.steps.size())
            throw pu::Error("Synthetic orientation not complete");
        if (mode == "--blocked") {
            bool refused = false;
            try { desk.open(2); } catch (const pu::DefinitionMismatch&) { refused = true; }
            if (!refused) throw pu::Error("Definition mismatch was not refused");
        } else {
            const auto second = desk.open(2);
            const auto& p = second.progress.value;
            if (pu::currentStep(p) != (mode == "--first" ? 1u : 7u))
                throw pu::Error("Unexpected saved Mission 003 position");
            if (p.steps[1].attempts.empty() || p.steps[1].attempts.front() != "SYNTHETIC final prediction" || p.steps[1].hintsUsed != 1)
                throw pu::Error("Prediction/hint not retained");
            if (second.progress.warnings.empty() == (mode == "--recovered"))
                throw pu::Error("Unexpected recovery state");
            if (pu::StudyDesk::report(second).find("No compiler pass") == std::string::npos)
                throw pu::Error("Report overstated authority");
        }
        std::cout << "PASS: final state " << mode << " verified with real course loader\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << "FAIL: " << e.what() << '\n'; return 1; }
}
