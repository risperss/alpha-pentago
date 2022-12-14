#include <cstdint>
#include <unordered_map>

#include "neural/minimax.h"

namespace pentago {
PositionLookup* clearedLookup(PositionLookup* lookup, Position position);
PositionLookup* smartClearedLookup(PositionLookup* lookup);

PositionHistory selfPlay();
PositionHistory randomSelfPlay();
}  // namespace pentago