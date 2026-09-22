#include "ProposalReview.h"
#include "ProposalParser.h"
#include <utility>

namespace pu::preview {
namespace {
ReviewState reviewState(State state) {
    switch (state) {
    case State::displayed: return ReviewState::displayed;
    case State::stale: return ReviewState::stale;
    case State::rejected: return ReviewState::rejected;
    }
    return ReviewState::rejected;
}
}
void ReviewSession::discard(ReviewState state) noexcept {
    selected_.clear();
    state_ = state;
}
ReviewReport ReviewSession::select(const std::string& document, const Snapshot& current) {
    // Invalidate first, including if validation/allocation throws; never retain an old display.
    discard(ReviewState::rejected);
    auto result = inspectSerialized(document, current);
    if (result.state != State::displayed) {
        state_ = reviewState(result.state);
        return {state_, std::move(result.text)};
    }
    selected_ = document; // F2 has already enforced the 64 KiB bound.
    state_ = ReviewState::proposed;
    return {state_, "PROPOSED: fixture selected in memory; preview required. No approval or write authority.\n"};
}
ReviewReport ReviewSession::preview(const Snapshot& current) {
    if (state_ != ReviewState::proposed && state_ != ReviewState::displayed)
        return {state_, "NO ACTIVE PROPOSAL: select a valid fixture before preview. Nothing applied or saved.\n"};
    // Mark invalid before work so an exception cannot leave displayed status behind.
    state_ = ReviewState::rejected;
    try {
        auto result = inspectSerialized(selected_, current);
        state_ = reviewState(result.state);
        if (state_ != ReviewState::displayed) selected_.clear();
        return {state_, std::move(result.text)};
    } catch (...) {
        selected_.clear();
        throw;
    }
}
ReviewReport ReviewSession::dismiss() {
    discard(ReviewState::dismissed);
    return {state_, "DISMISSED: selection cleared. Nothing applied or saved.\n"};
}
ReviewReport reviewFixtureDemo(ReviewFixture example) {
    const std::string document = R"({"format":1,"id":"u4-f1","path":"fixture/review-note.txt","baseRevision":"fixture-r1","changeClass":"fixture","before":"Review pending.\n","after":"Review displayed.\n","claim":"This wording is easier to read."})";
    ReviewSession session;
    const auto current = fixtureSnapshot();
    std::string transcript;
    auto append = [&](ReviewReport report) { transcript += report.text; return report.state; };
    append(session.select(document, current));
    append(session.preview(current));
    if (example == ReviewFixture::stale) {
        auto changed = current; changed.bytes += "Changed after display.";
        append(session.preview(changed));
        append(session.preview(current)); // Reverting data alone cannot resurrect a stale review.
        return {session.state(), std::move(transcript)};
    }
    if (example == ReviewFixture::rejectedReplacement) {
        append(session.select("{invalid", current));
        append(session.preview(current)); // Previous valid proposal is no longer available.
        return {session.state(), std::move(transcript)};
    }
    append(session.dismiss());
    append(session.preview(current));
    append(session.select(document, current));
    append(session.preview(current));
    return {session.state(), std::move(transcript)};
}
}
