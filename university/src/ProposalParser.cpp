#include "ProposalParser.h"
#include "mentor/Json.h"
#include <string_view>

namespace pu::preview {
namespace {
Report rejected() {
    return {State::rejected, "REJECTED: invalid or unsupported serialized fixture proposal. Nothing applied or saved.\n"};
}
// Called only after validating a flat object with seven strings and one number.
// Find that sole number outside strings, then compare its decimal value exactly.
// Json stores doubles: 1.00000000000000001 must not become format 1 by rounding.
bool exactFormatOne(const std::string& text) {
    std::size_t i = 0;
    while (i < text.size()) {
        if (text[i] == '"') {
            ++i;
            while (i < text.size() && text[i] != '"') {
                if (text[i] == '\\') ++i;
                ++i;
            }
        } else if (text[i] == '-' || (text[i] >= '0' && text[i] <= '9')) break;
        ++i;
    }
    if (i == text.size() || text[i] == '-') return false;
    int digits = 0, decimal = -1, one = -1;
    while (i < text.size() && (text[i] == '.' || (text[i] >= '0' && text[i] <= '9'))) {
        const char c = text[i++];
        if (c == '.') { decimal = digits; continue; }
        if (c != '0') {
            if (c != '1' || one != -1) return false;
            one = digits;
        }
        ++digits;
    }
    if (decimal == -1) decimal = digits;
    int exponent = 0;
    if (i < text.size() && (text[i] == 'e' || text[i] == 'E')) {
        ++i;
        bool negative = false;
        if (i < text.size() && (text[i] == '-' || text[i] == '+')) negative = text[i++] == '-';
        while (i < text.size() && text[i] >= '0' && text[i] <= '9') {
            exponent = exponent * 10 + (text[i++] - '0');
            if (exponent > 65536) return false; // Cannot cancel within a 64 KiB document.
        }
        if (negative) exponent = -exponent;
    }
    return one >= 0 && decimal - one - 1 + exponent == 0;
}
}
Report inspectSerialized(const std::string& document, const Snapshot& current) {
    if (document.size() > 64 * 1024) return rejected();
    try {
        const auto root = mentor::Json::parse(document);
        const auto& fields = root.object();
        constexpr std::string_view keys[] = {
            "id", "path", "baseRevision", "changeClass", "before", "after", "claim"
        };
        if (fields.size() != 8 || !root.has("format") || root.at("format").number() != 1.0)
            return rejected();
        for (auto key : keys) {
            const auto found = fields.find(std::string(key));
            if (found == fields.end() || !std::holds_alternative<std::string>(found->second.value))
                return rejected();
        }
        if (!exactFormatOne(document)) return rejected();
        const Proposal proposal{1, root.at("id").str(), root.at("path").str(),
            root.at("baseRevision").str(), root.at("changeClass").str(),
            root.at("before").str(), root.at("after").str(), root.at("claim").str()};
        return inspect(proposal, current); // F1 remains the policy and display authority.
    } catch (const pu::Error&) {
        // Codec diagnostics belong to Codex; neither those nor input bytes may escape.
        return rejected();
    }
}
Report serializedFixturePreview(SerializedFixture example) {
    // Deliberately serialized, not generated through the codec being tested.
    const std::string document = R"({"format":1,"id":"u4-f1","path":"fixture/review-note.txt","baseRevision":"fixture-r1","changeClass":"fixture","before":"Review pending.\n","after":"Review displayed.\n","claim":"This wording is easier to read."})";
    auto snapshot = fixtureSnapshot();
    if (example == SerializedFixture::stale) snapshot.bytes = "Changed after proposal creation.\n";
    return inspectSerialized(example == SerializedFixture::malformed ? document + " trailing" : document, snapshot);
}
}
