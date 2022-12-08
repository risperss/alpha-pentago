#include <cstdint>

#include "pentago/position.h"

namespace pen {

static const std::uint64_t kSidesMask = 0b010010101101010010010010101101010010;
static const std::uint64_t kCornersMask =
    0b101101000000101101101101000000101101;
static const std::uint64_t kCentresMask =
    0b000000010010000000000000010010000000;

int heuristic_value(Position position);

}  // namespace pen