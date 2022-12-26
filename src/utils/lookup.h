#include "neural/minimax.h"

namespace pentago {
namespace {
static PositionLookup* smartClearedLookup(PositionLookup* position_lookup) {
  PositionLookup* newLookup = new PositionLookup;

  for (auto& [hash, lookupItem] : *position_lookup) {
    if (lookupItem.value == kMaxPositionValue ||
        lookupItem.value == kMinPositionValue) {
      (*newLookup)[hash] = lookupItem;
    }
  }

  delete position_lookup;
  return newLookup;
}
}  // namespace

static PositionLookup* clearedLookup(PositionLookup* position_lookup,
                                     Position position) {
  if (position.GetPlyCount() + kMaxSearchDepth >= 9) {
    return smartClearedLookup(position_lookup);
  } else {
    delete position_lookup;
    return new PositionLookup;
  }
}
}  // namespace pentago
