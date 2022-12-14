#include <array>
#include <unordered_map>

#include "neural/heuristic.h"
#include "pentago/bitboard.h"
#include "pentago/position.h"

#pragma once

namespace pentago {
static const int kPositiveInfinity = INT_MAX;
static const int kNegativeInfinity = INT_MIN;
static const int kMaxPositionValue = 10000;
static const int kMinPositionValue = -10000;
static const int kMaxSearchDepth = 3;

static const HeuristicEvaluator defaultHeuristicEvaluator =
    HeuristicEvaluator({2, 3, 5, -1, 8, 15, 100, 50});

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
                    int* nodesVisited, const bool infiniteSearch);

ReturnValue minimax(Position position, PositionLookup* lookup,
                    int* nodesVisited, const bool infiniteSearch);
}  // namespace pentago