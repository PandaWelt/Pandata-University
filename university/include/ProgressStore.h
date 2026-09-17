#pragma once
#include "Mission.h"
#include <cstdint>
#include <filesystem>
namespace pu {
struct LoadedProgress {
    Progress value;
    std::uint64_t generation = 0;
    std::vector<std::string> warnings;
};
// Append-only snapshots. Never replaces an existing committed snapshot.
LoadedProgress loadProgress(const std::filesystem::path& directory, const Mission&);
void saveProgress(const std::filesystem::path& directory, const Mission&, LoadedProgress&);
}
