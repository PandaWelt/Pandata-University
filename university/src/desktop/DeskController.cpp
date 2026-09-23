#include "desktop/DeskController.h"
#include <chrono>
#include <sstream>

namespace pu::desktop {
DeskController::DeskController(const std::filesystem::path& root) : desk_(root), actions_(root) {
    desk_.validateDefinitions(); refresh(); resume();
}
void DeskController::refresh() { cards_ = desk_.catalog(); }
bool DeskController::select(std::size_t n) {
    return attempt([&] { auto candidate = desk_.open(n); lesson_ = std::move(candidate); message_ = "Lesson opened. No progress changed."; refresh(); });
}
bool DeskController::resume() {
    return attempt([&] { auto candidate = desk_.resume(); lesson_ = std::move(candidate); message_ = "Resumed saved position."; refresh(); });
}
bool DeskController::answer(const std::string& text) {
    return attempt([&] {
        if (!lesson_) throw Error("Choose an available lesson.");
        auto candidate = lesson_->progress;
        recordAnswer(lesson_->mission, candidate.value, text);
        desk_.save(*lesson_, std::move(candidate)); refresh();
        message_ = "Answer saved. This records your observation; it does not grade C++ correctness.";
    });
}
bool DeskController::next() {
    return attempt([&] {
        if (!lesson_) throw Error("Choose an available lesson.");
        auto candidate = lesson_->progress;
        advance(lesson_->mission, candidate.value);
        desk_.save(*lesson_, std::move(candidate)); refresh(); message_ = "Step recorded and position saved.";
    });
}
bool DeskController::hint() {
    return attempt([&] {
        if (!lesson_) throw Error("Choose an available lesson.");
        auto candidate = lesson_->progress;
        const auto text = revealHint(lesson_->mission, candidate.value);
        desk_.save(*lesson_, std::move(candidate)); message_ = text;
    });
}
bool DeskController::view(std::size_t n) {
    file_ = 0; fileText_.clear(); // A refused replacement must not show stale content as the new file.
    return attempt([&] { fileText_ = actions_.view(n); file_ = n; message_ = "Read-only file loaded. No edits or saves are available."; });
}
bool DeskController::checkCourse() {
    return attempt([&] { desk_.validateDefinitions(); refresh(); output_ = "PASS: course definitions and prerequisite graph validated.\nNo compiler pass, product completion or mastery inferred."; message_ = "Course check complete."; });
}
bool DeskController::start(Action action) {
    return attempt([&] {
        if (busy_) throw Error("An action is already running. Wait for its bounded result.");
        std::string name;
        switch (action) {
        case Action::UniversityCheck: name = "run-check"; break;
        case Action::GitStatus: name = "git-status"; break;
        case Action::GitDiff: name = "git-diff"; break;
        case Action::OpenVisualStudio: name = "open-vs"; break;
        default: throw Error("Unsupported desktop action.");
        }
        // Only this fixed enum maps to the previously reviewed repository adapter.
        job_ = std::async(std::launch::async, [actions = actions_, name] { return actions.execute(name); });
        busy_ = true; output_ = "Running " + name + " ...\nThe desktop stays responsive. Output appears when the bounded action completes.";
        message_ = "Action started; no lesson completion is granted.";
    });
}
void DeskController::poll() {
    if (!busy_ || job_.wait_for(std::chrono::seconds(0)) != std::future_status::ready) return;
    busy_ = false;
    const bool ok = attempt([&] {
        const auto report = job_.get(); std::ostringstream out;
        out << (report.ok() ? "PASS: " : "FAILED: ") << report.action << '\n';
        for (const auto& c : report.commands) {
            out << "Command: " << terminalText(c.executable.string());
            for (const auto& a : c.arguments) out << ' ' << jsonString(a);
            out << "\nWorking directory: " << terminalText(c.cwd.string()) << '\n' << terminalText(c.output);
            if (c.handedOff) out << "IDE handoff; completion not observed.\n";
            else out << "\nExit: " << c.exitCode << " | timeout " << c.timedOut << " | output limit " << c.outputLimited << '\n';
            if (!c.error.empty()) out << terminalText(c.error) << '\n';
        }
        if (!report.error.empty()) out << terminalText(report.error) << '\n';
        out << "Evidence: " << terminalText(report.evidence.string()) << "\nUniversity checks do not build or grade PandaBank.";
        output_ = out.str(); message_ = report.ok() ? "Repository action finished." : "Repository action failed; inspect output. Your lesson position is unchanged.";
    });
    if(!ok) output_ = "FAILED: " + message_;
}
std::string DeskController::report() const { return lesson_ ? StudyDesk::report(*lesson_) : "No accessible lesson."; }
std::string DeskController::history() const { return lesson_ ? StudyDesk::history(*lesson_) : "No accessible lesson."; }
}
