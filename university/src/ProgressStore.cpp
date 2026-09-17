#include "ProgressStore.h"
#include <algorithm>
#include <charconv>
#include <fstream>
#include <limits>

namespace pu {
namespace {
namespace fs = std::filesystem;
struct Scan {
    std::uint64_t latest = 0;
    std::vector<std::pair<std::uint64_t, fs::path>> committed;
    bool pending = false;
};
void noSymlinks(const fs::path& path) {
    fs::path part;
    for (const auto& component : fs::absolute(path)) {
        part /= component;
        if (fs::is_symlink(fs::symlink_status(part))) throw Error("progress path may not traverse symlinks");
    }
}
Scan scan(const fs::path& dir) {
    noSymlinks(dir); Scan s;
    if (!fs::exists(dir)) return s;
    if (!fs::is_directory(dir)) throw Error("progress location is not a directory");
    std::size_t count = 0;
    for (const auto& entry : fs::directory_iterator(dir)) {
        if (++count > 10000) throw Error("progress directory exceeds 10000 entries; archive it before continuing");
        const auto name = entry.path().filename().string();
        const bool pending = name.ends_with(".pup.tmp");
        if (!pending && !name.ends_with(".pup")) continue;
        if (entry.is_symlink() || !entry.is_regular_file()) throw Error("snapshot is not a regular file: " + name);
        const auto prefix = name.substr(0, name.size() - (pending ? 8 : 4));
        std::uint64_t n = 0;
        const auto [end, error] = std::from_chars(prefix.data(), prefix.data() + prefix.size(), n);
        if (error != std::errc{} || end != prefix.data() + prefix.size() || n == 0 || std::to_string(n) != prefix) throw Error("invalid snapshot filename: " + name);
        s.latest = std::max(s.latest, n);
        if (pending) s.pending = true;
        else s.committed.emplace_back(n, entry.path());
    }
    std::sort(s.committed.begin(),s.committed.end(),[](const auto& a,const auto& b){return a.first > b.first;});
    return s;
}
class Lock {
    fs::path path_;
public:
    explicit Lock(fs::path path) : path_(std::move(path)) {
        if (!fs::create_directory(path_)) throw Error("progress save is locked; close other instances. After a crash, preserve files and remove .save-lock only when no instance is running");
    }
    ~Lock() { std::error_code error; fs::remove(path_,error); }
    Lock(const Lock&) = delete;
    Lock& operator=(const Lock&) = delete;
};
}
LoadedProgress loadProgress(const std::filesystem::path& dir, const Mission& m) {
    const auto files = scan(dir);
    LoadedProgress loaded{freshProgress(m), files.latest, {}};
    if (files.pending) loaded.warnings.push_back("An unfinished .tmp snapshot was preserved and ignored.");
    for (const auto& [number, file] : files.committed) {
        (void)number;
        try {
            std::ifstream in(file,std::ios::binary); if (!in) throw Error("cannot open snapshot");
            loaded.value = parseProgress(m,in); return loaded;
        } catch (const DefinitionMismatch&) { throw; }
        catch (const Error& error) { loaded.warnings.push_back("Preserved invalid " + file.filename().string() + ": " + error.what()); }
    }
    if (!files.committed.empty()) throw Error("No valid progress snapshot. Nothing was reset or overwritten. Preserve this directory and restore a known-good snapshot before continuing.");
    return loaded;
}
void saveProgress(const std::filesystem::path& dir, const Mission& m, LoadedProgress& loaded) {
    const auto text = serializeProgress(m,loaded.value);
    noSymlinks(dir); std::filesystem::create_directories(dir);
    Lock lock(dir / ".save-lock");
    const auto files = scan(dir);
    if (files.latest != loaded.generation) throw Error("another session saved progress; restart and reload before writing");
    if (files.latest == std::numeric_limits<std::uint64_t>::max()) throw Error("snapshot sequence exhausted");
    const auto next = files.latest + 1;
    const auto final = dir / (std::to_string(next) + ".pup");
    const auto temporary = dir / (std::to_string(next) + ".pup.tmp");
    if (std::filesystem::exists(final) || std::filesystem::exists(temporary)) throw Error("snapshot path already exists");
    {
        std::ofstream out(temporary,std::ios::binary);
        if (!out) throw Error("cannot create progress snapshot");
        out << text; out.flush();
        if (!out) throw Error("progress write failed; previous snapshots preserved");
        out.close(); if (!out) throw Error("progress close failed; previous snapshots preserved");
    }
    std::filesystem::rename(temporary,final);
    loaded.generation = next;
}
}
