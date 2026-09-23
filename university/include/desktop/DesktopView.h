#pragma once
#include "desktop/DeskController.h"
#include <array>
#include <vector>
namespace pu::desktop {
// Replaceable Dear ImGui presentation. Core/store/controller have no graphics dependency.
class DesktopView {
    DeskController& desk_;
    std::array<char,241> answer_{};
    std::string identity_, draftError_, fileCache_, outputCache_, reportCache_;
    std::vector<char> fileBuffer_{'\0'}, outputBuffer_{'\0'}, reportBuffer_{'\0'};
    bool home_ = true;
    float scale_ = 1.0f;
    void overview();
    void lesson();
    void workspace();
public:
    explicit DesktopView(DeskController& desk) : desk_(desk) {}
    void draw();
    void showCurrentLesson() { home_ = false; }
    static void theme(float dpi);
};
}
