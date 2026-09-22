#pragma once
#include <string>

namespace pu::preview {
// Values only. No file, process, provider, approval, callback or progress capability.
struct Proposal {
    int format = 1;
    std::string id, path, baseRevision, changeClass, before, after, claim;
};
struct Snapshot { std::string path, revision, bytes; };
enum class State { displayed, stale, rejected };
struct Report { State state; std::string text; };
Report inspect(const Proposal&, const Snapshot&);
Proposal fixtureProposal();
Snapshot fixtureSnapshot();
Report fixturePreview(bool demonstrateStale = false);
}
