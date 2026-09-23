#pragma once
#include "StudyDesk.h"
#include "RepositoryActions.h"
#include <future>
#include <optional>

namespace pu::desktop {
enum class Action { UniversityCheck, GitStatus, GitDiff, OpenVisualStudio };
// No graphics types, provider, command textbox, editor or file-write service.
class DeskController {
    StudyDesk desk_;
    RepositoryActions actions_;
    std::optional<StudyLesson> lesson_;
    std::vector<LessonCard> cards_;
    std::future<ActionReport> job_;
    std::string message_, output_, fileText_;
    std::size_t file_ = 0;
    bool busy_ = false;
    void refresh();
    template<class F> bool attempt(F operation) {
        try { operation(); return true; }
        catch (const std::exception& e) { message_ = std::string("Not saved/completed: ") + e.what(); return false; }
    }
public:
    explicit DeskController(const std::filesystem::path& root);
    const auto& cards() const { return cards_; }
    const auto& lesson() const { return lesson_; }
    const auto& files() const { return actions_.files(); }
    const auto& fileText() const { return fileText_; }
    const auto& message() const { return message_; }
    const auto& output() const { return output_; }
    std::size_t selectedFile() const { return file_; }
    bool busy() const { return busy_; }
    bool select(std::size_t number);
    bool resume();
    bool answer(const std::string& text);
    bool next();
    bool hint();
    bool view(std::size_t number);
    bool checkCourse();
    bool start(Action action);
    void poll();
    std::string report() const;
    std::string history() const;
    static constexpr const char* mentorState = "Offline - native context-only read isolation is unproven. No context is sent.";
};
}
