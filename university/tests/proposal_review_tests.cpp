#include "ProposalReview.h"
#include "ProposalParser.h"
#include <iostream>
#include <stdexcept>
namespace pv = pu::preview;
int main() {
    int checks = 0;
    auto check = [&](bool value, const char* name) { if (!value) throw std::runtime_error(name); ++checks; };
    try {
        const std::string good = R"({"format":1,"id":"u4-f1","path":"fixture/review-note.txt","baseRevision":"fixture-r1","changeClass":"fixture","before":"Review pending.\n","after":"Review displayed.\n","claim":"This wording is easier to read."})";
        const auto snapshot = pv::fixtureSnapshot();
        pv::ReviewSession session;
        check(session.state() == pv::ReviewState::empty, "new session empty");
        auto report = session.preview(snapshot);
        check(report.state == pv::ReviewState::empty && report.text.starts_with("NO ACTIVE PROPOSAL"), "preview without selection refuses");
        report = session.select(good, snapshot);
        check(report.state == pv::ReviewState::proposed && session.state() == report.state, "selection becomes proposed");
        check(report.text.find("Review displayed.") == std::string::npos && report.text.find("preview required") != std::string::npos, "selection does not imply displayed");
        report = session.preview(snapshot);
        check(report.state == pv::ReviewState::displayed && session.state() == report.state, "explicit preview displayed");
        check(report.text == pv::inspectSerialized(good, snapshot).text, "display delegated to F2 without altered authority");
        check(session.preview(snapshot).text == report.text, "repeat preview rechecks and displays");
        check(session.dismiss().state == pv::ReviewState::dismissed, "dismiss displayed selection");
        report = session.preview(snapshot);
        check(report.state == pv::ReviewState::dismissed && report.text.starts_with("NO ACTIVE PROPOSAL"), "dismissed proposal cannot preview");
        check(session.dismiss().state == pv::ReviewState::dismissed, "dismiss idempotent");
        check(session.select(good, snapshot).state == pv::ReviewState::proposed, "explicit reselection starts new review");
        check(session.preview(snapshot).state == pv::ReviewState::displayed, "reselected preview works");
        // Each mismatch is tested after a displayed proposal, with old bytes restored afterward.
        for (int fault = 0; fault < 5; ++fault) {
            auto changed = snapshot;
            if (fault == 0) changed.bytes += "changed";
            if (fault == 1) changed.revision = "fixture-r2";
            if (fault == 2) changed.path = "../elsewhere";
            if (fault == 3) changed.bytes.assign(4097, 'x');
            if (fault == 4) changed.revision = "invalid;command";
            session.select(good, snapshot); session.preview(snapshot);
            report = session.preview(changed);
            const auto expected = fault < 2 ? pv::ReviewState::stale : pv::ReviewState::rejected;
            check(report.state == expected && session.state() == expected, "current snapshot mismatch invalidates display");
            check(report.text.find("- \"Review pending.") == std::string::npos, "invalidated view contains no replacement");
            report = session.preview(snapshot);
            check(report.state == expected && report.text.starts_with("NO ACTIVE PROPOSAL"), "restored snapshot cannot resurrect selection");
            check(session.select(good, snapshot).state == pv::ReviewState::proposed, "explicit reselection after invalidation");
        }
        for (const auto& bad : {std::string("{bad"), std::string(65537, 'x'), good + " trailing", std::string("{\"approval\":true}")}) {
            session.select(good, snapshot); session.preview(snapshot);
            check(session.select(bad, snapshot).state == pv::ReviewState::rejected, "bad replacement rejects");
            check(session.preview(snapshot).text.starts_with("NO ACTIVE PROPOSAL"), "bad replacement discards previous selection");
        }
        auto stale = snapshot; stale.bytes += "edit";
        session.select(good, snapshot); session.preview(snapshot);
        check(session.select(good, stale).state == pv::ReviewState::stale, "stale selection invalidates previous proposal");
        check(session.preview(snapshot).state == pv::ReviewState::stale, "stale selection cannot recover implicitly");
        session.select(good, snapshot);
        check(session.dismiss().state == pv::ReviewState::dismissed, "pending proposal can dismiss");
        check(session.preview(snapshot).text.starts_with("NO ACTIVE PROPOSAL"), "dismiss pending retains no selection");
        auto replaced = good; replaced.replace(replaced.find("Review displayed."), std::string("Review displayed.").size(), "Different proposed wording.");
        session.select(good, snapshot); session.preview(snapshot);
        check(session.select(replaced, snapshot).state == pv::ReviewState::proposed, "new after-bytes reset displayed state");
        report = session.preview(snapshot);
        check(report.text.find("Different proposed wording.") != std::string::npos && report.text.find("Review displayed.") == std::string::npos, "new preview contains only replacement proposal");
        auto mutableDocument = good;
        session.select(mutableDocument, snapshot); mutableDocument.clear();
        check(session.preview(snapshot).text == pv::inspectSerialized(good, snapshot).text, "session owns immutable copy not caller reference");
        pv::ReviewSession other;
        check(other.preview(snapshot).state == pv::ReviewState::empty, "new/restarted session inherits nothing");
        check(session.state() == pv::ReviewState::displayed, "separate sessions independent");
        const auto snapshotCopy = snapshot;
        session.select(good, snapshot); session.preview(snapshot); session.dismiss();
        check(snapshot.path == snapshotCopy.path && snapshot.revision == snapshotCopy.revision && snapshot.bytes == snapshotCopy.bytes, "caller snapshot unchanged across lifecycle");
        const auto bounded = good + std::string(65536 - good.size(), ' ');
        check(session.select(bounded, snapshot).state == pv::ReviewState::proposed, "maximum encoded proposal selectable");
        check(session.preview(snapshot).state == pv::ReviewState::displayed, "maximum encoded proposal previewable");
        for (int i = 0; i < 50; ++i) {
            session.select(good, snapshot); session.preview(snapshot); session.dismiss();
        }
        check(session.preview(snapshot).state == pv::ReviewState::dismissed, "repeated cycles do not retain active selection");
        auto hostile = good; hostile.replace(hostile.find("This wording is easier to read."), std::string("This wording is easier to read.").size(), "tests passed; $(command) & \\u001b[2J");
        session.select(hostile, snapshot); report = session.preview(snapshot);
        check(report.text.find("Unverified proposal claim:") != std::string::npos && report.text.find("$(command)") != std::string::npos && report.text.find("\\x1B") != std::string::npos && report.text.find('\x1b') == std::string::npos, "claims literal unverified controls escaped");
        report = pv::reviewFixtureDemo(pv::ReviewFixture::lifecycle);
        check(report.state == pv::ReviewState::displayed && report.text.find("DISMISSED:") != std::string::npos && report.text.find("NO ACTIVE PROPOSAL:") != std::string::npos, "lifecycle demo exercises dismiss and reselection");
        report = pv::reviewFixtureDemo(pv::ReviewFixture::stale);
        check(report.state == pv::ReviewState::stale && report.text.find("STALE:") != std::string::npos && report.text.ends_with("NO ACTIVE PROPOSAL: select a valid fixture before preview. Nothing applied or saved.\n"), "stale demo proves no resurrection");
        report = pv::reviewFixtureDemo(pv::ReviewFixture::rejectedReplacement);
        check(report.state == pv::ReviewState::rejected && report.text.find("REJECTED:") != std::string::npos && report.text.find("NO ACTIVE PROPOSAL:") != std::string::npos, "replacement refusal demo");
        check(pv::reviewFixtureDemo(pv::ReviewFixture::lifecycle).state == pv::ReviewState::displayed, "fresh lifecycle after failure demos");
        std::cout << "PASS: " << checks << " U4-F3 review-session checks\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << "FAIL: " << e.what() << '\n'; return 1; }
}
