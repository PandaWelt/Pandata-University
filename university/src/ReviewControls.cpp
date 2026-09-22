#include "ReviewControls.h"
#include <utility>
namespace pu::preview {
namespace {
const char* stateName(ReviewState state) {
    switch (state) {
    case ReviewState::empty: return "empty";
    case ReviewState::proposed: return "proposed";
    case ReviewState::displayed: return "displayed";
    case ReviewState::stale: return "stale";
    case ReviewState::rejected: return "rejected";
    case ReviewState::dismissed: return "dismissed";
    }
    return "unknown";
}
}
ControlReport ReviewControls::execute(std::string_view command) {
    if (command.size() <= 32) {
        if (command == "review-status")
            return {true, std::string("REVIEW STATUS: ") + stateName(session_.state()) +
                " (last operation only; not approval or a real-file freshness check).\n"};
        if (command == "review-dismiss") return {true, session_.dismiss().text};
        if (command == "review-select") {
            const std::string fixture = R"({"format":1,"id":"u4-f1","path":"fixture/review-note.txt","baseRevision":"fixture-r1","changeClass":"fixture","before":"Review pending.\n","after":"Review displayed.\n","claim":"This wording is easier to read."})";
            auto result = session_.select(fixture, fixtureSnapshot());
            return {result.state == ReviewState::proposed, std::move(result.text)};
        }
        if (command == "review-preview") {
            auto result = session_.preview(fixtureSnapshot());
            return {result.state == ReviewState::displayed, std::move(result.text)};
        }
    }
    return {false, "REJECTED: use exactly review-select, review-preview, review-status or review-dismiss; no arguments.\n"};
}
ControlReport reviewControlsDemo(bool demonstrateRefusal) {
    ReviewControls controls;
    std::string output;
    if (demonstrateRefusal) {
        output += controls.execute("review-select").text;
        const auto refused = controls.execute("review-apply");
        output += refused.text;
        output += controls.execute("review-status").text;
        return {refused.ok, std::move(output)};
    }
    bool ok = true;
    int step = 0;
    for (auto command : {"review-status", "review-select", "review-preview", "review-dismiss", "review-preview", "review-status", "review-select", "review-preview"}) {
        const auto result = controls.execute(command);
        ok = ok && (result.ok == (step != 4)); // Dismissed preview must refuse.
        output += result.text;
        ++step;
    }
    return {ok, std::move(output)};
}
}
