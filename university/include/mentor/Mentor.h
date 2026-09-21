#pragma once
#include "Mission.h"
#include "mentor/Channel.h"
#include "mentor/Json.h"
#include <functional>
namespace pu::mentor {
struct Result { bool available=false;std::string state="offline",detail,model,plan,answer,threadId,turnId; Json compatibility; };
// No mutable mission, progress, repository adapter, or filesystem access enters the protocol controller.
std::string context(const Mission&,const Progress&);
std::vector<std::string> serverArguments();
std::string checkSchema(const std::filesystem::path&,const std::string& codexVersion="");
// Local diagnostic only: recognizing a profile API does not authorize a model request.
Json inspectCompatibility(const std::filesystem::path&,const std::string& codexVersion);
Result interact(Channel&,const std::string& preview,const std::string& isolatedCwd,bool ask,const std::string& threadSandbox="readOnly");
std::filesystem::path record(const std::filesystem::path&,const std::string&,const Result&);
Result run(const std::filesystem::path& repository,const std::string& preview,bool ask);
}
