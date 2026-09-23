#pragma once
#include "ProgressStore.h"
#include <filesystem>
#include <string>
#include <vector>
namespace pu {
struct StudyLesson {
    std::size_t number = 0;
    Mission mission;
    LoadedProgress progress;
};
struct LessonCard {
    std::size_t number = 0, completed = 0, total = 0;
    std::string id, title, state, detail;
};
// Fixed reviewed catalog; definitions cannot grant file/process/write capabilities.
class StudyDesk {
    std::filesystem::path root_;
    Mission definition(std::size_t number) const;
    LoadedProgress progress(const Mission&) const;
    void prerequisites(std::size_t number, const Mission&) const;
public:
    explicit StudyDesk(std::filesystem::path root);
    void validateDefinitions() const;
    std::vector<LessonCard> catalog() const;
    StudyLesson open(std::size_t number) const;
    StudyLesson resume() const;
    void save(StudyLesson&, LoadedProgress candidate) const;
    std::string home() const;
    static std::string history(const StudyLesson&);
    static std::string reviewStep(const StudyLesson&, std::size_t number);
    static std::string report(const StudyLesson&);
    static std::string feedback();
};
// Canonical decimal selection only, no paths, arguments, trimming or shell syntax.
std::size_t studyNumber(const std::string&);
}
