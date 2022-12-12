#include <unordered_map>

#include "pentago/bitboard.h"
#include "pentago/position.h"

#pragma once

namespace pen {
static const std::int8_t POS_INFINITY = INT8_MAX;
static const std::int8_t NEG_INFINITY = INT8_MIN;
static const std::int8_t MAX_POSITION_VALUE = 100;
static const std::int8_t MIN_POSITION_VALUE = -100;
static const int DEPTH = 4;

struct ReturnValue {
  std::int8_t value;
  Move move;
  std::uint8_t plyCount;
};

// To reduce size of lookup tables
struct LookupItem {
  std::int8_t value;
  std::uint8_t plyCount;
};

using PositionLookup = std::unordered_map<std::uint64_t, LookupItem>;

ReturnValue minimax(Position position, Move prevMove, int depth,
                    std::int8_t alpha, std::int8_t beta, bool maximizingPlayer,
                    PositionLookup* lookup, int* nodesVisited);

ReturnValue minimax(Position position, PositionLookup* lookup,
                    int* nodesVisited);
}  // namespace pen