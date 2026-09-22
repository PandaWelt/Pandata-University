#include "ReviewControls.h"
#include <iostream>
#include <stdexcept>
#include <vector>
namespace pv = pu::preview;
int main() {
    int checks = 0;
    auto check = [&](bool yes, const char* name) { if (!yes) throw std::runtime_error(name); ++checks; };
    try {
        pv::ReviewControls controls;
        controls.execute("review-select"); controls.execute("review-preview");
        const auto status = controls.execute("review-status").text;
        for (const std::string command : {"review-select", "review-preview", "review-status", "review-dismiss"}) {
            const std::vector<std::string> invalid{
                " " + command, command + " ", "\t" + command, command + "\t", command + "\r",
                command + "\n", command + std::string(1, '\0'), command + "\v", command + "\f",
                std::string("\xEF\xBB\xBF") + command, command + "\xC2\xA0", command + "\xE2\x80\x8B",
                command + "\xE2\x80\xA8", command + "\x1B[0m", command + " argument",
                command + " ../file", command + " & echo x", command + "|cmd", command + ";cmd",
                command + "$(cmd)", '"' + command + '"', command + "  ", command + "\b",
                command.substr(0, 7) + " " + command.substr(7)
            };
            for (const auto& bad : invalid) {
                const auto result = controls.execute(bad);
                check(!result.ok && result.text.starts_with("REJECTED:"), "malformed command rejected");
                check(controls.execute("review-status").text == status, "malformed command cannot mutate session");
            }
            auto upper = command; upper[0] = 'R';
            check(!controls.execute(upper).ok, "altered casing refused");
            check(controls.execute("review-status").text == status, "case refusal cannot mutate session");
        }
        check(controls.execute("review-preview").ok, "valid command after all refusals works");
        check(controls.execute("review-dismiss").ok, "exact dismiss works");
        check(!controls.execute("review-preview").ok, "exact dismiss clears selection");
        std::cout << "PASS: " << checks << " command-boundary checks\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << "FAIL: " << e.what() << '\n'; return 1; }
}
