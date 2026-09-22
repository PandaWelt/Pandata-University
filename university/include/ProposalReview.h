#pragma once
#include "ProposalPreview.h"

namespace pu::preview {
enum class ReviewState { empty, proposed, displayed, stale, rejected, dismissed };
struct ReviewReport { ReviewState state; std::string text; };
// One owned, bounded proposal. No approval/apply API, callbacks or external access.
class ReviewSession {
public:
    ReviewReport select(const std::string& document, const Snapshot& current);
    ReviewReport preview(const Snapshot& current);
    ReviewReport dismiss();
    // Historical state only, not a freshness or approval token.
    ReviewState state() const noexcept { return state_; }
private:
    std::string selected_;
    ReviewState state_ = ReviewState::empty;
    void discard(ReviewState state) noexcept;
};
enum class ReviewFixture { lifecycle, stale, rejectedReplacement };
ReviewReport reviewFixtureDemo(ReviewFixture example);
}
