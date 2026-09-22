// Standalone validation helper: no University link, paths, shell calls or persistence.
#include <cstdio>
#include <iostream>
#include <string>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif
namespace {
const std::string expected = "review-status\r\nreview-select\r\nreview-preview\r\nreview-apply\r\nreview-dismiss\r\nreview-preview\r\nreview-status\r\nquit\r\n";
std::string hex(const std::string& bytes) {
    constexpr const char* digits = "0123456789ABCDEF";
    std::string out;
    for (unsigned char c : bytes) { out += digits[c >> 4]; out += digits[c & 15]; }
    return out;
}
}
int main(int argc, char** argv) {
    if (argc != 2 || (std::string(argv[1]) != "--verify" && std::string(argv[1]) != "--diagnose")) {
        std::cerr << "Use --verify or --diagnose; reads bounded stdin only.\n"; return 2;
    }
#ifdef _WIN32
    if (_setmode(_fileno(stdin), _O_BINARY) == -1) { std::cerr << "Cannot set binary stdin mode.\n"; return 2; }
#endif
    std::string bytes;
    char c;
    while (std::cin.get(c)) {
        if (bytes.size() == 4096) { std::cerr << "FAIL: stdin exceeds 4096 bytes.\n"; return 2; }
        bytes += c;
    }
    if (!std::cin.eof()) { std::cerr << "FAIL: stdin read error.\n"; return 2; }
    const bool exact = bytes == expected;
    // All untrusted data encoded as hex; JSON is machine-readable, no raw controls.
    std::cout << "{\"exact\":" << (exact ? "true" : "false") << ",\"bytes\":" << bytes.size()
              << ",\"hex\":\"" << hex(bytes) << "\",\"lines\":[";
    std::size_t start = 0; bool first = true;
    while (start < bytes.size()) {
        const auto end = bytes.find('\n', start);
        const auto line = bytes.substr(start, end == std::string::npos ? end : end - start + 1);
        if (!first) std::cout << ',';
        first = false;
        std::cout << "{\"bytes\":" << line.size() << ",\"hex\":\"" << hex(line) << "\"}";
        if (end == std::string::npos) break;
        start = end + 1;
    }
    std::cout << "]}\n";
    return std::string(argv[1]) == "--diagnose" ? 0 : (exact ? 0 : 1);
}
