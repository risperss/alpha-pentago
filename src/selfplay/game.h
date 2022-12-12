#include <cstdint>
#include <unordered_map>

#include "neural/minimax.h"

namespace pen {
PositionLookup* clearedLookup(PositionLookup* lookup, Position position);
PositionLookup* smartClearedLookup(PositionLookup* lookup);

void selfPlay();
}  // namespace pen