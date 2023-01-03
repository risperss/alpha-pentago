#include "heuristic.h"

#include <array>
#include <cstdint>

#include "pentago/position.h"
#include "utils/bitops.h"

namespace pentago {

HeuristicEvaluator::HeuristicEvaluator(Genome genome) { genome_ = genome; }

float HeuristicEvaluator::value(Position position) const {
  const std::uint64_t our_board = position.GetBoard().our_pieces().as_int();
  const std::uint64_t their_board = position.GetBoard().their_pieces().as_int();

  float value = 0;

  value += goodSquaresScore(our_board) - goodSquaresScore(their_board);
  value += centralityScore(our_board) - centralityScore(their_board);
  value += fourOfFiveScore(our_board, their_board) -
           fourOfFiveScore(their_board, our_board);

  value -= position.GetPlyCount() * 0.1;

  return value;
}

float HeuristicEvaluator::goodSquaresScore(const std::uint64_t board_) const {
  const std::uint64_t kSidesMask = 0b010010101101010010010010101101010010;
  const std::uint64_t kCornersMask = 0b101101000000101101101101000000101101;
  const std::uint64_t kCentresMask = 0b000000010010000000000000010010000000;

  float score = genome_.at(0) * count(board_ & kSidesMask) +
                genome_.at(1) * count(board_ & kCornersMask) +
                genome_.at(2) * count_few(board_ & kCentresMask);

  return score;
}

float HeuristicEvaluator::centralityScore(const std::uint64_t board_) const {
  std::uint64_t outermostRing = 0b111111100001100001100001100001111111;
  std::uint64_t middleRing = 0b000000011110010010010010011110000000;
  std::uint64_t innermostRing = 0b000000000000001100001100000000000000;

  float score = genome_.at(3) * count(board_ & outermostRing) +
                genome_.at(4) * count(board_ & middleRing) +
                genome_.at(5) * count_few(board_ & innermostRing);

  return score;
}

float HeuristicEvaluator::fourOfFiveScore(
    const std::uint64_t our_board_, const std::uint64_t their_board_) const {
  float score = 0;

  for (std::uint64_t mask : kWinningMasks) {
    if (count_few(our_board_ & mask) == 4) {
      if ((their_board_ & mask) == 0) {
        score += genome_.at(6);
      } else {
        score += genome_.at(7);
      }
    }
  }

  return score;
}

}  // namespace pentago