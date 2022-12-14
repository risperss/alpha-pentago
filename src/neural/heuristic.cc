#include "heuristic.h"

#include <cstdint>

#include "pentago/position.h"
#include "utils/bitops.h"

namespace pen {
namespace {
// TODO: see if it's better to make longer list of all possible masks rather
// than using count
// Also, reference variable from board.cc, don't ctrl-C ctrl-V lol.
std::uint64_t kWinningMasks[] = {
    0x000000000000001F, 0x000000000000003E, 0x0000000001041041,
    0x0000000041041040, 0x00000000000007C0, 0x0000000000000F80,
    0x0000000002082082, 0x0000000082082080, 0x000000000001F000,
    0x000000000003E000, 0x0000000004104104, 0x0000000104104100,
    0x00000000007C0000, 0x0000000000F80000, 0x0000000008208208,
    0x0000000208208200, 0x000000001F000000, 0x000000003E000000,
    0x0000000010410410, 0x0000000410410400, 0x00000007C0000000,
    0x0000000F80000000, 0x0000000020820820, 0x0000000820820800,
    0x0000000010204081, 0x0000000810204080, 0x0000000002108420,
    0x0000000042108400, 0x0000000408102040, 0x0000000020408102,
    0x0000000001084210, 0x0000000084210800};
}  // namespace

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

  for (std::uint64_t& mask : kWinningMasks) {
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