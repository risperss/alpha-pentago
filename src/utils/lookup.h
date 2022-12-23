#include "neural/minimax.h"

namespace pentago {
static PositionLookup* smartClearedLookup(PositionLookup* lookup) {
  PositionLookup* newLookup = new PositionLookup;

  for (auto& [hash, lookupItem] : *lookup) {
    if (lookupItem.value == kMaxPositionValue ||
        lookupItem.value == kMinPositionValue) {
      (*newLookup)[hash] = lookupItem;
    }
  }

  delete lookup;
  return newLookup;
}

static PositionLookup* clearedLookup(PositionLookup* lookup,
                                     Position position) {
  if (position.GetPlyCount() + kMaxSearchDepth >= 9) {
    return smartClearedLookup(lookup);
  } else {
    delete lookup;
    return new PositionLookup;
  }
}
}  // namespace pentago