#include <array>
#include <cfloat>
#include <unordered_map>

#include "neural/heuristic.h"
#include "pentago/bitboard.h"
#include "pentago/position.h"

#pragma once

namespace pentago {
static const float kPositiveInfinity = FLT_MAX;
static const float kNegativeInfinity = -FLT_MAX;
static const int kMaxSearchDepth = 3;

static const std::array<float, kNumWeights> kDefaultWeights = {
    -290.39, -258.93, -254.83, -263.00, -253.64, -249.61, -252.08, -271.19};

struct ReturnValue {
  float value;
  Move move;
  std::uint8_t plyCount;
};

// To reduce size of lookup tables
struct LookupItem {
  float value;
  std::uint8_t plyCount;
};

using PositionLookup = std::unordered_map<std::uint64_t, LookupItem>;

ReturnValue minimax(Position position, Move prevMove, int depth, float alpha,
                    float beta, bool maximizingPlayer, PositionLookup* lookup,
                    int* nodesVisited, HeuristicEvaluator* evaluator);

ReturnValue minimax(Position position, int depth, PositionLookup* lookup,
                    int* nodesVisited, HeuristicEvaluator* evaluator);
}  // namespace pentago