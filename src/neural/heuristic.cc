#include "heuristic.h"

#include <cstdint>

#include "pentago/position.h"
#include "utils/bitops.h"

namespace pen {
int heuristic_value(Position position) {
  std::uint64_t ours = position.GetBoard().our_pieces().as_int();
  std::uint64_t theirs = position.GetBoard().their_pieces().as_int();

  int ourScore = 2 * count(ours & kSidesMask) + 3 * count(ours & kCornersMask) +
                 5 * count(ours & kCentresMask);

  int theirScore = 2 * count(theirs & kSidesMask) +
                   3 * count(theirs & kCornersMask) +
                   5 * count(theirs & kCentresMask);

  int score = ourScore - theirScore;

  if (position.IsBlackToMove()) {
    score = -score;
  }

  return score;
}
}  // namespace pen