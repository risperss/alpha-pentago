#include "heuristic.h"

#include <cstdint>

#include "pentago/position.h"
#include "utils/bitops.h"

namespace pen {
int heuristic_value(Position position) {
  const std::uint64_t our_board = position.GetBoard().our_pieces().as_int();
  const std::uint64_t their_board = position.GetBoard().their_pieces().as_int();

  int value = 0;

  value += goodSquaresScore(our_board) - goodSquaresScore(their_board);
  value += centralityScore(our_board) - centralityScore(their_board);
  value += fourOfFiveScore(our_board, their_board);

  if (position.IsBlackToMove()) {
    value = -value;
  }

  return value;
}

int goodSquaresScore(const std::uint64_t board_) {
  const std::uint64_t kSidesMask = 0b010010101101010010010010101101010010;
  const std::uint64_t kCornersMask = 0b101101000000101101101101000000101101;
  const std::uint64_t kCentresMask = 0b000000010010000000000000010010000000;

  int score = 2 * count(board_ & kSidesMask) +
              3 * count(board_ & kCornersMask) +
              5 * count(board_ & kCentresMask);

  return score;
}

int centralityScore(const std::uint64_t board_) {
  std::uint64_t outermostRing = 0b111111100001100001100001100001111111;
  std::uint64_t middleRing = 0b000000011110010010010010011110000000;
  std::uint64_t innermostRing = 0b000000000000001100001100000000000000;

  int score = -1 * count(board_ & outermostRing) +
              8 * count(board_ & middleRing) +
              15 * count(board_ & innermostRing);

  return score;
}

int fourOfFiveScore(const std::uint64_t our_board_,
                    const std::uint64_t their_board_) {
  int score = 0;

  for (std::uint64_t mask : pen::kWinningMasks) {
    if (count_few(our_board_ & mask) == 4) {
      if ((their_board_ & mask) == 0) {
        score += 100;
      } else {
        score += 50;
      }
    } else if (count_few(their_board_ & mask) == 4) {
      if ((our_board_ & mask) == 0) {
        score -= 100;
      } else {
        score -= 50;
      }
    }
  }

  return score;
}
}  // namespace pen