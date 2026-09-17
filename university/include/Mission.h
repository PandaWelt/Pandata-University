#pragma once
#include <cstddef>
#include <istream>
#include <stdexcept>
#include <string>
#include <utility>
#include <vector>

namespace pu {
struct Error : std::runtime_error { using std::runtime_error::runtime_error; };
struct DefinitionMismatch : Error { using Error::Error; };
enum class StepKind { Explain, Predict, Inspect, ShortAnswer, Review, Checkpoint };
enum class Gate { Acknowledge, Response };
struct Step {
    std::string id, title, why, action, expected, more, professional;
    StepKind kind = StepKind::Explain;
    Gate gate = Gate::Acknowledge;
    std::vector<std::pair<std::string, std::string>> syntax;
    std::vector<std::string> hints;
};
struct Mission {
    std::string id, revision, title, fingerprint;
    std::vector<std::string> prerequisites, readOnly, allowed;
    std::vector<Step> steps;
};
struct StepProgress {
    bool complete = false;
    std::size_t hintsUsed = 0;
    std::vector<std::string> attempts;
};
struct Progress {
    std::string missionId, fingerprint;
    std::vector<StepProgress> steps;
};
struct PilotCompletion { std::string id, evidence, provenance; };
Mission loadMission(std::istream& input);
std::vector<PilotCompletion> loadPilot(std::istream& input);
void checkPrerequisites(const Mission&, const std::vector<PilotCompletion>&);
Progress freshProgress(const Mission&);
void validateProgress(const Mission&, const Progress&);
std::size_t currentStep(const Progress&);
void recordAnswer(const Mission&, Progress&, const std::string&);
void advance(const Mission&, Progress&);
std::string revealHint(const Mission&, Progress&);
std::string serializeProgress(const Mission&, const Progress&);
Progress parseProgress(const Mission&, std::istream&);
std::string readText(std::istream& input);
}
