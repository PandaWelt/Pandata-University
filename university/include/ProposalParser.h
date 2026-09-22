#pragma once
#include "ProposalPreview.h"

namespace pu::preview {
// Pure data adapter. No input stream, target lookup, execution or persistence.
Report inspectSerialized(const std::string& document, const Snapshot& current);
enum class SerializedFixture { valid, malformed, stale };
Report serializedFixturePreview(SerializedFixture example);
}
