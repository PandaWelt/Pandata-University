#include "ReviewControls.h"
#include <iostream>
#include <stdexcept>
namespace pv = pu::preview;
int main() {
    int checks = 0;
    auto check = [&](bool yes, const char* why) { if (!yes) throw std::runtime_error(why); ++checks; };
    try {
        pv::ReviewControls controls;
        check(controls.execute("review-status").text.find("empty") != std::string::npos, "initial status empty");
        check(!controls.execute("review-preview").ok, "no selection cannot preview");
        check(controls.execute("review-select").text.starts_with("PROPOSED:"), "select fixture");
        check(controls.execute("review-status").text.find("proposed") != std::string::npos, "status does not auto-preview");
        auto preview = controls.execute("review-preview");
        check(preview.ok && preview.text.starts_with("DISPLAYED:"), "preview selected fixture");
        check(preview.text.find("not a Git check") != std::string::npos && preview.text.find("Unverified proposal claim:") != std::string::npos, "advisory meaning preserved");
        const auto status = controls.execute("review-status").text;
        check(status.find("displayed") != std::string::npos && status.find("not approval") != std::string::npos, "status historical not approval");
        for (const auto& command : {std::string("review-apply"), std::string("review-select ../secret"), std::string("review-select C:\\secret"), std::string("review-preview;cmd"), std::string("review-select & echo x"), std::string("review-status\nreview-select"), std::string("review-select "), std::string("review-select\0x",15), std::string("REVIEW-SELECT"), std::string(33,'x'), std::string(65536,'x'), std::string("\x1b[2J"), std::string(""), std::string("git-status")}) {
            const auto refused = controls.execute(command);
            check(!refused.ok && refused.text.starts_with("REJECTED:") && refused.text.find('\x1b') == std::string::npos, "exact commands only with safe diagnostics");
            check(controls.execute("review-status").text == status, "refusal does not execute or alter selection");
        }
        check(controls.execute("review-preview").text == preview.text, "valid preview after refused commands");
        check(controls.execute("review-select").text.starts_with("PROPOSED:"), "reselect resets displayed state");
        check(controls.execute("review-dismiss").text.starts_with("DISMISSED:"), "dismiss proposed");
        check(!controls.execute("review-preview").ok, "dismissed selection cannot resurrect");
        check(controls.execute("review-status").text.find("dismissed") != std::string::npos, "no-active preview retains dismissed status");
        check(controls.execute("review-dismiss").ok, "repeat dismiss harmless");
        controls.execute("review-select"); controls.execute("review-preview");
        check(controls.execute("review-dismiss").ok && !controls.execute("review-preview").ok, "dismiss displayed selection");
        pv::ReviewControls restarted;
        check(restarted.execute("review-status").text.find("empty") != std::string::npos, "restart stores no session");
        check(controls.execute("review-status").text.find("dismissed") != std::string::npos, "sessions independent");
        check(pv::reviewControlsDemo(false).ok, "compiled successful controls demo");
        check(!pv::reviewControlsDemo(true).ok, "compiled forbidden apply demo");
        check(pv::reviewControlsDemo(true).text.find("REVIEW STATUS: proposed") != std::string::npos, "forbidden operation leaves only proposed state");
        check(pv::reviewControlsDemo(false).ok, "fresh controls after refusal");
        std::cout << "PASS: " << checks << " U4-F4 review-control checks\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << "FAIL: " << e.what() << '\n'; return 1; }
}
