#pragma once
#include "ProposalReview.h"
#include <string_view>
namespace pu::preview {
struct ControlReport { bool ok; std::string text; };
// Fixed fixture UI actions only; no action callback or input/file/provider interface.
class ReviewControls {
public:
    ControlReport execute(std::string_view command);
private:
    ReviewSession session_;
};
ControlReport reviewControlsDemo(bool demonstrateRefusal);
}
