#include "ProposalPreview.h"

namespace pu::preview {
namespace {
constexpr const char* fixturePath = "fixture/review-note.txt";
constexpr std::size_t textLimit = 4096;
bool revisionValid(const std::string& value) {
    return !value.empty() && value.size() <= 64 &&
        value.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_") == std::string::npos;
}
std::string quotedBytes(const std::string& value) {
    constexpr const char* hex = "0123456789ABCDEF";
    std::string out = "\"";
    for (unsigned char c : value) {
        if (c == '\n') out += "\\n";
        else if (c == '\r') out += "\\r";
        else if (c == '\t') out += "\\t";
        else if (c == '\\' || c == '"') { out += '\\'; out += static_cast<char>(c); }
        else if (c < 32 || c >= 127) { out += "\\x"; out += hex[c >> 4]; out += hex[c & 15]; }
        else out += static_cast<char>(c);
    }
    return out + '"';
}
}
Report inspect(const Proposal& p, const Snapshot& current) {
    if (p.format != 1 || p.id != "u4-f1" || p.changeClass != "fixture")
        return {State::rejected, "REJECTED: unsupported fixture identity, format or change class. Nothing applied.\n"};
    if (p.path != fixturePath || current.path != fixturePath)
        return {State::rejected, "REJECTED: only the fixed logical fixture path is allowed. No file was opened.\n"};
    if (!revisionValid(p.baseRevision) || !revisionValid(current.revision) ||
        p.before.size() > textLimit || p.after.size() > textLimit ||
        current.bytes.size() > textLimit || p.claim.size() > 256)
        return {State::rejected, "REJECTED: invalid revision or oversized fixture data. Nothing applied.\n"};
    if (p.baseRevision != current.revision || p.before != current.bytes)
        return {State::stale, "STALE: supplied revision or before-bytes changed. Preview refused; nothing applied.\n"};
    return {State::displayed,
        "DISPLAYED: U4-F1 in-memory fixture only. No approval or write authority.\n"
        "No files, Git, processes or AI were consulted by this preview.\n"
        "Logical path: fixture/review-note.txt (not opened)\n"
        "Supplied fixture revision: " + quotedBytes(p.baseRevision) + " (not a Git check)\n"
        "Exact replacement view; quoted bytes preserve whitespace and escape controls:\n"
        "- " + quotedBytes(p.before) + "\n+ " + quotedBytes(p.after) + "\n"
        "Unverified proposal claim: " + quotedBytes(p.claim) + "\n"
        "No compiler/test result is established by this proposal.\n"
        "Nothing applied or saved. No rollback needed.\n"};
}
Proposal fixtureProposal() {
    return {1, "u4-f1", fixturePath, "fixture-r1", "fixture",
        "Review pending.\n", "Review displayed.\n", "This wording is easier to read."};
}
Snapshot fixtureSnapshot() { return {fixturePath, "fixture-r1", "Review pending.\n"}; }
Report fixturePreview(bool demonstrateStale) {
    auto current = fixtureSnapshot();
    if (demonstrateStale) current.bytes = "Changed after proposal creation.\n";
    return inspect(fixtureProposal(), current);
}
}
