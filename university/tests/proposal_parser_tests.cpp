#include "ProposalParser.h"
#include <iostream>
#include <stdexcept>
namespace pv = pu::preview;
namespace {
// Build test JSON independently of the production decoder/encoder.
std::string quoted(const std::string& bytes) {
    const char* hex = "0123456789abcdef";
    std::string out = "\"";
    for (unsigned char c : bytes) {
        if (c == '"' || c == '\\') { out += '\\'; out += static_cast<char>(c); }
        else if (c < 32) { out += "\\u00"; out += hex[c >> 4]; out += hex[c & 15]; }
        else out += static_cast<char>(c);
    }
    return out + '"';
}
std::string document(const pv::Proposal& p) {
    return "{\"format\":1,\"id\":" + quoted(p.id) + ",\"path\":" + quoted(p.path) +
        ",\"baseRevision\":" + quoted(p.baseRevision) + ",\"changeClass\":" + quoted(p.changeClass) +
        ",\"before\":" + quoted(p.before) + ",\"after\":" + quoted(p.after) + ",\"claim\":" + quoted(p.claim) + "}";
}
std::string replace(std::string text, const std::string& from, const std::string& to) {
    const auto at = text.find(from);
    if (at == std::string::npos) throw std::runtime_error("bad test fixture");
    text.replace(at, from.size(), to); return text;
}
}
int main() {
    int checks = 0;
    auto check = [&](bool value, const char* name) {
        if (!value) throw std::runtime_error(name);
        ++checks;
    };
    try {
        const auto p = pv::fixtureProposal();
        const auto snapshot = pv::fixtureSnapshot();
        const auto good = document(p);
        const auto original = good;
        auto parse = [&](const std::string& text) { return pv::inspectSerialized(text, snapshot); };
        auto rejects = [&](const std::string& text) { return parse(text).state == pv::State::rejected; };
        check(parse(good).text == pv::inspect(p, snapshot).text, "valid JSON matches F1 view exactly");
        check(good == original && snapshot.bytes == p.before && snapshot.revision == p.baseRevision, "inputs unchanged");
        check(parse(" \t\r\n" + good + "\n").state == pv::State::displayed, "JSON whitespace accepted");
        for (auto value : {"1", "1.0", "1e0", "1E+0", "0.1e1", "100e-2", "0.0100E+2"})
            check(parse(replace(good, "\"format\":1", std::string("\"format\":") + value)).state == pv::State::displayed, "equivalent exact numeric one");
        for (auto value : {"0", "2", "-1", "1.1", "1.00000000000000001", "0.99999999999999999", "1e99999", "NaN", "01", "true", "null", "\"1\"", "{}", "[]"})
            check(rejects(replace(good, "\"format\":1", std::string("\"format\":") + value)), "non-format or rounded value refused");
        for (auto key : {"id", "path", "baseRevision", "changeClass", "before", "after", "claim"}) {
            const auto start = good.find(std::string("\"") + key + "\":");
            const auto colon = good.find(':', start);
            // Replace only the key: simultaneously tests missing and unknown fields.
            check(rejects(replace(good, std::string("\"") + key + "\":", "\"unknown\":")), "all required keys enforced");
            std::size_t end = colon + 2;
            while (end < good.size()) {
                if (good[end] == '\\') { end += 2; continue; }
                if (good[end] == '"') break;
                ++end;
            }
            for (auto wrong : {"null", "true", "1", "[]", "{}"}) {
                auto bad = good; bad.replace(colon + 1, end - colon, wrong);
                check(rejects(bad), "every string field rejects coercion/nesting");
            }
        }
        for (auto bad : {"", "{}", "[]", "null", "1", "\"text\"", "{", "{\"format\":1,}", "/* comment */"})
            check(rejects(bad), "malformed or nonobject rejected");
        check(rejects(replace(good, "\"format\":1,", "")), "missing numeric field");
        for (auto extra : {"\"command\":\"echo injected\"", "\"approval\":true", "\"snapshot\":{}", "\"validation\":\"passed\"", "\"format\":1", "\"f\\u006frmat\":1"})
            check(rejects(good.substr(0, good.size()-1) + ',' + extra + '}'), "extra/duplicate/decoded-duplicate keys rejected");
        check(rejects(good + good), "multiple documents rejected");
        check(rejects(good + std::string(1, '\0')), "trailing NUL rejected");
        check(rejects(std::string(60, '[') + "0" + std::string(60, ']')), "excessive nesting rejected");
        check(rejects(replace(good, "\"claim\":", "\"claim\":") + " trailing"), "trailing tokens rejected");
        for (auto escape : {"\\q", "\\uD800", "\\uDC00", "\\uD800\\u0041", "\\uZZZZ"})
            check(rejects(replace(good, quoted(p.claim), std::string("\"") + escape + '"')), "invalid escape/surrogate rejected");
        for (const auto& invalid : {std::string("\xc0\xaf"), std::string("\x80"), std::string("\xed\xa0\x80"), std::string("\xf4\x90\x80\x80"), std::string("\xe2\x82"), std::string("\x1b[2J")})
            check(rejects(replace(good, quoted(p.claim), '"' + invalid + '"')), "invalid UTF8/raw controls rejected");
        check(parse(replace(good, quoted(p.claim), "\"\\uD83D\\uDC3C\"")).text.find("\\xF0\\x9F\\x90\\xBC") != std::string::npos, "valid surrogate pair escaped on display");
        check(parse(replace(good, quoted(p.claim), "\"\xe5\xad\xa6\"")).text.find("\\xE5\\xAD\\xA6") != std::string::npos, "valid UTF8 escaped on display");
        check(parse(good + std::string(65536-good.size(), ' ')).state == pv::State::displayed, "exact encoded limit accepted");
        check(rejects(good + std::string(65537-good.size(), ' ')), "over encoded limit rejected");
        for (auto length : {4096, 4097}) {
            auto candidate = p; candidate.after.assign(length, '\0');
            const auto report = parse(document(candidate));
            check(report.state == (length == 4096 ? pv::State::displayed : pv::State::rejected), "escaped NUL decoded byte limit");
            candidate = p; candidate.before.assign(length, 'b'); auto current = snapshot; current.bytes = candidate.before;
            check(pv::inspectSerialized(document(candidate), current).state == (length == 4096 ? pv::State::displayed : pv::State::rejected), "before and current decoded limit");
        }
        for (auto length : {256, 257}) {
            auto candidate = p; candidate.claim.assign(length, 'x');
            check(parse(document(candidate)).state == (length == 256 ? pv::State::displayed : pv::State::rejected), "claim byte limit");
        }
        for (auto length : {64, 65}) {
            auto candidate = p; candidate.baseRevision.assign(length, 'r'); auto current = snapshot; current.revision = candidate.baseRevision;
            check(pv::inspectSerialized(document(candidate), current).state == (length == 64 ? pv::State::displayed : pv::State::rejected), "revision bound");
        }
        for (auto path : {"../secret", "C:\\secret", "/etc/passwd", "fixture/other", ""}) {
            auto candidate = p; candidate.path = path; check(rejects(document(candidate)), "fixed logical path enforced");
        }
        auto candidate = p; candidate.id = "u4-f2"; check(rejects(document(candidate)), "identity not expanded");
        candidate = p; candidate.changeClass = "protected-policy"; check(rejects(document(candidate)), "class not expanded");
        candidate = p; candidate.baseRevision = "r;cmd"; check(rejects(document(candidate)), "revision charset enforced");
        auto current = snapshot; current.path = "elsewhere"; check(pv::inspectSerialized(good, current).state == pv::State::rejected, "independent snapshot path enforced");
        current = snapshot; current.bytes += "edit"; check(pv::inspectSerialized(good, current).state == pv::State::stale, "stale bytes");
        current = snapshot; current.revision += "2"; check(pv::inspectSerialized(good, current).state == pv::State::stale, "stale revision");
        candidate = p; candidate.after = "$(touch marker) & echo injected | command"; candidate.claim = "tests passed";
        auto report = parse(document(candidate));
        check(report.text.find(candidate.after) != std::string::npos && report.text.find("Unverified proposal claim: \"tests passed\"") != std::string::npos, "shell text literal and claims unverified");
        candidate.after = std::string("\x1b[2J\r\n\t") + '\0' + "\\\"";
        report = parse(document(candidate));
        check(report.text.find("\\x1B[2J\\r\\n\\t\\x00\\\\\\\"") != std::string::npos && report.text.find('\x1b') == std::string::npos && report.text.find('\0') == std::string::npos, "decoded controls escaped losslessly");
        const auto refusal = parse("{\"injected\":\"\x1b[2J\"}");
        check(refusal.text == parse("").text && refusal.text.find("Codex") == std::string::npos, "fixed safe error without input or provider diagnostics");
        check(parse(good).text == pv::inspect(p, snapshot).text && good == original, "fresh success after refusals with inputs preserved");
        check(pv::serializedFixturePreview(pv::SerializedFixture::valid).state == pv::State::displayed, "compiled valid demo");
        check(pv::serializedFixturePreview(pv::SerializedFixture::malformed).state == pv::State::rejected, "compiled malformed demo");
        check(pv::serializedFixturePreview(pv::SerializedFixture::stale).state == pv::State::stale, "compiled stale demo");
        check(pv::serializedFixturePreview(pv::SerializedFixture::valid).state == pv::State::displayed, "compiled fresh demo");
        std::cout << "PASS: " << checks << " U4-F2 checks (in-memory parser only)\n";
        return 0;
    } catch (const std::exception& e) { std::cerr << "FAIL: " << e.what() << '\n'; return 1; }
}
