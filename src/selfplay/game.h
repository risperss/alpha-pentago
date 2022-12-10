#include <cstdint>
#include <unordered_map>

#include "neural/minimax.h"

namespace pen {
using PositionLookup = std::unordered_map<std::uint64_t, ReturnValue>;

PositionLookup* smartClearedLookup(PositionLookup* lookup);

void selfPlay();
void vsHuman();
}  // namespace pen