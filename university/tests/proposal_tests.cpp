#include "ProposalPreview.h"
#include <iostream>
#include <stdexcept>
namespace pv = pu::preview;
int main() {
    int count = 0;
    auto check = [&](bool yes, const char* why) {
        if (!yes) throw std::runtime_error(why);
        ++count;
    };
    try {
        const auto p = pv::fixtureProposal();
        const auto s = pv::fixtureSnapshot();
        auto result = pv::inspect(p, s);
        check(result.state == pv::State::displayed, "matching snapshot previews");
        check(result.text.find("- \"Review pending.\\n\"\n+ \"Review displayed.\\n\"") != std::string::npos, "exact replacement view");
        check(result.text.find("not a Git check") != std::string::npos, "fixture cannot claim real Git verification");
        check(p.before == s.bytes && s.bytes == "Review pending.\n", "preview leaves supplied values unchanged");
        auto changed = s; changed.bytes += "another edit";
        check(pv::inspect(p, changed).state == pv::State::stale, "changed bytes invalidate preview");
        changed = s; changed.revision = "fixture-r2";
        check(pv::inspect(p, changed).state == pv::State::stale, "changed revision invalidates preview");
        check(pv::fixturePreview(true).state == pv::State::stale, "deliberate stale fixture fails safely");
        check(pv::fixturePreview().state == pv::State::displayed, "stale demo does not alter next fixture");
        for (const auto* path : {"../secret", "/tmp/secret", "C:\\secret", "fixture/../review-note.txt", "fixture/other.txt", ""}) {
            auto bad = p; bad.path = path;
            check(pv::inspect(bad, s).state == pv::State::rejected, "out-of-scope logical path rejected");
        }
        changed = s; changed.path = "other";
        check(pv::inspect(p, changed).state == pv::State::rejected, "snapshot target must match fixed fixture");
        for (const auto* kind : {"content", "engine", "product", "protected-policy", ""}) {
            auto bad = p; bad.changeClass = kind;
            check(pv::inspect(bad, s).state == pv::State::rejected, "non-fixture class rejected");
        }
        auto bad = p; bad.format = 2;
        check(pv::inspect(bad, s).state == pv::State::rejected, "unknown format rejected");
        bad = p; bad.id = "other";
        check(pv::inspect(bad, s).state == pv::State::rejected, "unknown identity rejected");
        bad = p; bad.baseRevision.clear();
        check(pv::inspect(bad, s).state == pv::State::rejected, "missing revision rejected");
        bad = p; bad.baseRevision = "revision;command";
        check(pv::inspect(bad, s).state == pv::State::rejected, "invalid revision refused");
        bad = p; bad.after.assign(4097, 'x');
        check(pv::inspect(bad, s).state == pv::State::rejected, "oversized proposed bytes refused");
        bad = p; bad.before.assign(4097, 'x');
        check(pv::inspect(bad, s).state == pv::State::rejected, "oversized original bytes refused");
        changed = s; changed.bytes.assign(4097, 'x');
        check(pv::inspect(p, changed).state == pv::State::rejected, "oversized supplied snapshot refused");
        bad = p; bad.claim.assign(257, 'x');
        check(pv::inspect(bad, s).state == pv::State::rejected, "oversized evidence claim refused");
        bad = p; bad.after.assign(4096, 'x');
        check(pv::inspect(bad, s).state == pv::State::displayed, "exact size bound accepted");
        bad = p; bad.after = "$(touch marker) & echo fake | command"; bad.claim = "tests passed";
        result = pv::inspect(bad, s);
        check(result.state == pv::State::displayed && result.text.find(bad.after) != std::string::npos, "shell-looking body is literal data");
        check(result.text.find("Unverified proposal claim: \"tests passed\"") != std::string::npos, "proposal cannot certify its own tests");
        bad.after = std::string("\x1b[2J\r\n") + char(0) + char(0xFF) + "\\\"";
        result = pv::inspect(bad, s);
        check(result.text.find("\\x1B[2J\\r\\n\\x00\\xFF\\\\\\\"") != std::string::npos, "control and non-ASCII bytes escaped losslessly");
        check(result.text.find(char(27)) == std::string::npos && result.text.find(char(0)) == std::string::npos, "no raw escape or NUL enters terminal");
        std::cout << "PASS: " << count << " U4-F1 checks (in-memory preview only)\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << "FAIL: " << e.what() << '\n'; return 1; }
}
