#include <unordered_map>

#include "pentago/bitboard.h"
#include "pentago/position.h"

#pragma once

namespace pen {
static const int MAX_POSITION_VALUE = 1000;
static const int DEPTH = 3;

struct ReturnValue {
  int value;
  Move move;
  int plyCount;
};

using PositionLookup = std::unordered_map<std::uint64_t, ReturnValue>;

ReturnValue minimax(Position position, Move prevMove, int depth, int alpha,
                    int beta, PositionLookup* lookup);

ReturnValue minimax(Position position, PositionLookup* lookup);
}  // namespace pen