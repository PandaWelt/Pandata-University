#pragma once
#include <filesystem>
#include <string>
#include <vector>

namespace pu {
// Platform adapter; only RepositoryActions chooses commands in the study UI.
struct ProcessResult {
    std::filesystem::path executable, cwd;
    std::vector<std::string> arguments;
    std::string output, error;
    int exitCode = -1;
    bool started = false, timedOut = false, outputLimited = false;
    long long elapsedMs = 0;
    bool handedOff = false;
    bool ok() const { return started && exitCode == 0 && !timedOut && !outputLimited && error.empty(); }
};
ProcessResult runProcess(const std::filesystem::path&, const std::vector<std::string>&,
                         const std::filesystem::path&, int timeoutMs = 120000);
std::filesystem::path findTool(const std::string&, const std::filesystem::path& repository);
ProcessResult launchVisualStudio(const std::filesystem::path&, const std::filesystem::path&, const std::filesystem::path&);
std::string terminalText(const std::string&);
std::string jsonString(const std::string&);
void rejectLinks(const std::filesystem::path&);
struct ActionReport {
    std::string action, error;
    std::vector<ProcessResult> commands;
    std::filesystem::path evidence;
    bool ok() const;
};
class RepositoryActions {
    std::filesystem::path root_;
    std::vector<std::string> files_;
    void save(ActionReport&) const;
public:
    explicit RepositoryActions(std::filesystem::path root);
    const std::vector<std::string>& files() const { return files_; }
    std::string view(std::size_t oneBased) const;
    // Fixed allowlist, not a public shell or editable command string.
    ActionReport execute(const std::string& action) const;
};
}
