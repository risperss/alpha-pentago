#include <cstdint>
#include <unordered_map>

#include "neural/minimax.h"

namespace pentago {
PositionLookup* clearedLookup(PositionLookup* lookup, Position position);
PositionLookup* smartClearedLookup(PositionLookup* lookup);

void selfPlay();
}  // namespace pentago