#include "Mission.h"
#include <algorithm>
#include <charconv>
#include <cstdint>
#include <iomanip>
#include <map>
#include <set>
#include <sstream>
#include <string_view>

namespace pu {
namespace {
constexpr std::size_t MaxText = 1024 * 1024;
void require(bool condition, const std::string& message) { if (!condition) throw Error(message); }
bool clean(const std::string& s) {
    return std::all_of(s.begin(), s.end(), [](unsigned char c) { return c >= 32 && c != 127; });
}
bool nonblank(const std::string& s) { return s.find_first_not_of(' ') != std::string::npos; }
bool identifier(const std::string& s) {
    return !s.empty() && s.size() <= 80 && std::all_of(s.begin(), s.end(), [](unsigned char c) {
        return (c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '-' || c == '_';
    });
}
std::string quoted(std::istringstream& line) {
    line >> std::ws;
    require(line.peek() == '"', "expected a quoted string");
    std::string result;
    require(bool(line >> std::quoted(result)), "unterminated quoted string");
    require(clean(result), "control characters are not allowed");
    return result;
}
std::size_t number(std::istringstream& line) {
    std::string token; line >> token;
    std::size_t n = 0;
    const auto [end, error] = std::from_chars(token.data(), token.data() + token.size(), n);
    require(!token.empty() && error == std::errc{} && end == token.data() + token.size(), "expected unsigned integer");
    return n;
}
void endLine(std::istringstream& line) {
    line >> std::ws; require(line.eof(), "unexpected trailing fields");
}
void path(const std::string& s) {
    require(!s.empty() && s.front() != '/' && s.find_first_of("\\:") == std::string::npos, "expected repository-relative slash path");
    std::istringstream parts(s); std::string part;
    while (std::getline(parts, part, '/')) require(!part.empty() && part != "." && part != "..", "unsafe relative path component");
}
std::string fingerprint(const std::string& bytes) {
    // Change detector, NOT a security hash or authentication mechanism.
    std::uint64_t h = 14695981039346656037ULL;
    for (unsigned char c : bytes) { h ^= c; h *= 1099511628211ULL; }
    std::ostringstream out; out << std::hex << h; return out.str();
}
struct Lines {
    std::istringstream input;
    std::size_t index = 0;
    explicit Lines(const std::string& text) : input(text) {}
    bool next(std::istringstream& line, std::string& key) {
        std::string raw;
        while (std::getline(input, raw)) {
            ++index;
            if (!raw.empty() && raw.back() == '\r') raw.pop_back();
            require(raw.size() <= 8192 && clean(raw), "line too long or contains control characters");
            if (raw.find_first_not_of(' ') == std::string::npos || raw[raw.find_first_not_of(' ')] == '#') continue;
            line.clear(); line.str(raw); line >> key; return true;
        }
        return false;
    }
};
void unique(std::set<std::string>& seen, const std::string& key) { require(seen.insert(key).second, "duplicate field or ID: " + key); }
}
std::string readText(std::istream& input) {
    std::string text; char c;
    while (input.get(c)) { require(text.size() < MaxText, "file exceeds 1 MiB limit"); text += c; }
    require(input.eof() && !input.bad(), "file read failed");
    return text;
}
Mission loadMission(std::istream& input) {
    const auto text = readText(input);
    Lines lines(text); std::istringstream line; std::string key;
    Mission m; std::set<std::string> ids, fields, prerequisites, paths;
    bool header = false, finished = false, inside = false;
    try {
        while (lines.next(line, key)) {
            require(!finished, "content after finish");
            if (!header) {
                require(key == "mission" && number(line) == 1, "expected mission version 1");
                m.id = quoted(line); m.revision = quoted(line); m.title = quoted(line);
                require(identifier(m.id) && identifier(m.revision) && nonblank(m.title), "invalid mission identity/title");
                header = true;
            } else if (key == "requires") {
                require(!inside && m.steps.empty(), "requires belongs before steps");
                auto id = quoted(line); require(identifier(id) && id != m.id, "invalid prerequisite"); unique(prerequisites, id); m.prerequisites.push_back(id);
            } else if (key == "scope") {
                require(!inside && m.steps.empty(), "scope belongs before steps");
                auto access = quoted(line); auto file = quoted(line); path(file); unique(paths, file);
                require(access == "read" || access == "edit", "scope must be read or edit");
                (access == "read" ? m.readOnly : m.allowed).push_back(file);
            } else if (key == "step") {
                require(!inside && m.steps.size() < 64, "nested step or too many steps");
                Step step; step.id = quoted(line); require(identifier(step.id), "invalid step ID"); unique(ids, step.id);
                const auto kind = quoted(line), gate = quoted(line); step.title = quoted(line);
                const std::map<std::string, StepKind> kinds{{"Explain",StepKind::Explain},{"Predict",StepKind::Predict},{"Inspect",StepKind::Inspect},{"ShortAnswer",StepKind::ShortAnswer},{"Review",StepKind::Review},{"Checkpoint",StepKind::Checkpoint}};
                require(kinds.count(kind) == 1, "unsupported step kind: " + kind);
                step.kind = kinds.at(kind); require(gate == "ack" || gate == "response", "unsupported completion gate");
                step.gate = gate == "ack" ? Gate::Acknowledge : Gate::Response;
                if (step.kind == StepKind::Predict || step.kind == StepKind::ShortAnswer)
                    require(step.gate == Gate::Response, "Predict/ShortAnswer requires response gate");
                if (step.kind == StepKind::Explain || step.kind == StepKind::Checkpoint)
                    require(step.gate == Gate::Acknowledge, "Explain/Checkpoint requires ack gate");
                require(nonblank(step.title), "empty step title");
                m.steps.push_back(step); fields.clear(); inside = true;
            } else if (key == "end") {
                require(inside, "end outside step");
                for (const auto* required : {"why","action","expect"}) require(fields.count(required) == 1, std::string("step missing ") + required);
                inside = false;
            } else if (key == "finish") {
                require(!inside && !m.steps.empty(), "finish requires complete steps"); finished = true;
            } else {
                require(inside, "unknown mission directive: " + key);
                auto& step = m.steps.back(); auto value = quoted(line); require(nonblank(value), "empty step field");
                if (key == "hint") { require(step.hints.size() < 7, "at most seven hints"); step.hints.push_back(value); }
                else if (key == "syntax") {
                    unique(fields, "syntax:" + value); auto meaning = quoted(line); require(nonblank(meaning), "empty syntax definition"); step.syntax.emplace_back(value, meaning);
                } else {
                    unique(fields, key);
                    if (key == "why") step.why = value;
                    else if (key == "action") step.action = value;
                    else if (key == "expect") step.expected = value;
                    else if (key == "more") step.more = value;
                    else if (key == "professional") step.professional = value;
                    else throw Error("unsupported step field: " + key);
                }
            }
            endLine(line);
        }
        require(header && finished && !inside, "incomplete mission; expected end/finish");
    } catch (const Error& e) { throw Error("mission line " + std::to_string(lines.index) + ": " + e.what()); }
    std::string canonical;
    for (std::size_t i = 0; i < text.size(); ++i) {
        if (text[i] == '\r' && i + 1 < text.size() && text[i+1] == '\n') continue;
        canonical += text[i];
    }
    m.fingerprint = fingerprint(canonical); return m;
}
std::vector<PilotCompletion> loadPilot(std::istream& input) {
    Lines lines(readText(input)); std::istringstream line; std::string key; std::set<std::string> ids;
    std::vector<PilotCompletion> result;
    require(lines.next(line, key) && key == "pilot" && number(line) == 1, "expected pilot version 1"); endLine(line);
    while (lines.next(line, key)) {
        require(key == "completed", "unknown pilot record");
        PilotCompletion entry{quoted(line),quoted(line),quoted(line)}; endLine(line);
        require(identifier(entry.id) && nonblank(entry.provenance), "invalid pilot record"); path(entry.evidence); unique(ids, entry.id); result.push_back(entry);
    }
    return result;
}
void checkPrerequisites(const Mission& m, const std::vector<PilotCompletion>& pilot) {
    for (const auto& id : m.prerequisites) require(std::any_of(pilot.begin(), pilot.end(), [&](const auto& p) { return p.id == id; }), "prerequisite not reviewed complete: " + id);
}
Progress freshProgress(const Mission& m) { return {m.id, m.fingerprint, std::vector<StepProgress>(m.steps.size())}; }
std::size_t currentStep(const Progress& p) {
    const auto it = std::find_if(p.steps.begin(), p.steps.end(), [](const auto& s) { return !s.complete; });
    return static_cast<std::size_t>(it - p.steps.begin());
}
void validateProgress(const Mission& m, const Progress& p) {
    if (p.missionId != m.id || p.fingerprint != m.fingerprint) throw DefinitionMismatch("mission bytes changed or wrong mission; preserve progress and request explicit migration");
    require(p.steps.size() == m.steps.size(), "progress step count mismatch");
    const auto active = currentStep(p);
    for (std::size_t i = 0; i < p.steps.size(); ++i) {
        const auto& s = p.steps[i]; const auto& step = m.steps[i];
        require(s.hintsUsed <= step.hints.size() && s.attempts.size() <= 100, "invalid hints/attempt count");
        if (i > active) require(!s.complete && s.hintsUsed == 0 && s.attempts.empty(), "out-of-order progress");
        if (step.gate == Gate::Acknowledge) require(s.attempts.empty(), "ack step cannot contain answers");
        for (const auto& answer : s.attempts) require(nonblank(answer) && clean(answer) && answer.size() <= 240, "answer must be 1-240 printable bytes");
        if (s.complete && step.gate == Gate::Response) require(!s.attempts.empty(), "completed response step has no evidence");
    }
}
void recordAnswer(const Mission& m, Progress& p, const std::string& text) {
    validateProgress(m, p); const auto i = currentStep(p);
    require(i < p.steps.size(), "prepared steps already complete");
    require(m.steps[i].gate == Gate::Response, "this step uses next, not an answer");
    require(nonblank(text) && clean(text) && text.size() <= 240, "use a short answer: 1-240 printable bytes");
    require(p.steps[i].attempts.size() < 100, "attempt limit reached; review before continuing"); p.steps[i].attempts.push_back(text);
}
void advance(const Mission& m, Progress& p) {
    validateProgress(m, p); const auto i = currentStep(p); require(i < p.steps.size(), "prepared steps already complete");
    if (m.steps[i].gate == Gate::Response) require(!p.steps[i].attempts.empty(), "answer this step before next");
    p.steps[i].complete = true;
}
std::string revealHint(const Mission& m, Progress& p) {
    validateProgress(m, p); const auto i = currentStep(p); require(i < p.steps.size(), "prepared steps already complete");
    auto& used = p.steps[i].hintsUsed; require(used < m.steps[i].hints.size(), "no further prepared hints; keep your observations for review");
    return m.steps[i].hints[used++];
}
std::string serializeProgress(const Mission& m, const Progress& p) {
    validateProgress(m, p); std::ostringstream out;
    out << "progress 1 " << std::quoted(p.missionId) << ' ' << std::quoted(p.fingerprint) << '\n';
    for (std::size_t i = 0; i < p.steps.size(); ++i) {
        const auto& s = p.steps[i]; out << "record " << std::quoted(m.steps[i].id) << ' ' << s.complete << ' ' << s.hintsUsed << '\n';
        for (const auto& a : s.attempts) out << "attempt " << std::quoted(a) << '\n';
    }
    out << "finish\n"; return out.str();
}
Progress parseProgress(const Mission& m, std::istream& input) {
    Lines lines(readText(input)); std::istringstream line; std::string key;
    require(lines.next(line,key) && key == "progress" && number(line) == 1, "expected progress version 1");
    Progress p{quoted(line),quoted(line),{}}; endLine(line);
    if (p.missionId != m.id || p.fingerprint != m.fingerprint) throw DefinitionMismatch("progress belongs to another mission revision; no automatic reset");
    bool done = false;
    while (lines.next(line,key)) {
        require(!done, "content after progress finish");
        if (key == "record") {
            const auto id = quoted(line); require(p.steps.size() < m.steps.size() && id == m.steps[p.steps.size()].id, "unexpected progress step ID/order");
            const auto complete = number(line), hints = number(line); require(complete <= 1, "invalid completion flag");
            p.steps.push_back({complete == 1, hints, {}});
        } else if (key == "attempt") {
            require(!p.steps.empty() && p.steps.back().attempts.size() < 100, "unexpected attempt"); p.steps.back().attempts.push_back(quoted(line));
        } else if (key == "finish") done = true;
        else throw Error("unknown progress field: " + key);
        endLine(line);
    }
    require(done, "truncated progress; missing finish"); validateProgress(m,p); return p;
}
}
