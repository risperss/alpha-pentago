#include <array>
#include <cstdint>

#include "pentago/position.h"

#pragma once

namespace pentago {

static const std::uint64_t kWinningMasks[] = {
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

static const int kNumWeights = 8;

class HeuristicEvaluator {
 public:
  HeuristicEvaluator() = default;
  HeuristicEvaluator(std::array<float, kNumWeights> weights);

  float value(Position position) const;

  float goodSquaresScore(const std::uint64_t board_) const;

  float centralityScore(const std::uint64_t board_) const;

  float fourOfFiveScore(const std::uint64_t our_board_,
                        const std::uint64_t their_board_) const;

 private:
  std::array<float, kNumWeights> weights_;
};

}  // namespace pentago