#include <unordered_map>

#include "pentago/bitboard.h"
#include "pentago/position.h"

#pragma once

namespace pen {
static const int POS_INFINITY = INT_MAX;
static const int NEG_INFINITY = INT_MIN;
static const int MAX_POSITION_VALUE = 100000;
static const int MIN_POSITION_VALUE = -100000;
static const int DEPTH = 4;

struct ReturnValue {
  int value;
  Move move;
  std::uint8_t plyCount;
};

// To reduce size of lookup tables
struct LookupItem {
  int value;
  std::uint8_t plyCount;
};

using PositionLookup = std::unordered_map<std::uint64_t, LookupItem>;

ReturnValue minimax(Position position, Move prevMove, int depth, int alpha,
                    int beta, bool maximizingPlayer, PositionLookup* lookup,
                    int* nodesVisited);

ReturnValue minimax(Position position, PositionLookup* lookup,
                    int* nodesVisited);
}  // namespace pen