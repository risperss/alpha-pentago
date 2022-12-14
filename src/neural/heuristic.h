#include <cstdint>

#include "pentago/position.h"

namespace pen {

int heuristic_value(Position position);

int goodSquaresScore(const std::uint64_t board_);

int centralityScore(const std::uint64_t board_);

int fourOfFiveScore(const std::uint64_t our_board_,
                    const std::uint64_t their_board_);

}  // namespace pen