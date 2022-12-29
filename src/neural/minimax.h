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
    0.67168461, 0.10707687, 0.06840958, -0.29951185,
    0.21427413, 0.33170923, 0.68111354, -0.04503335};

struct Value {
  float value;
  short ply_count;
};

struct Policy : Value {
  Move move;
};

using PositionLookup = std::unordered_map<std::uint64_t, Value>;

Policy minimax(Position position, Move prevMove, int depth, float alpha,
               float beta, bool maximizingPlayer,
               PositionLookup* position_lookup, int* nodesVisited,
               HeuristicEvaluator evaluator);

Policy minimax(Position position, int depth, PositionLookup* position_lookup,
               int* nodesVisited, HeuristicEvaluator evaluator);

std::pair<Value, bool> lookupPosition(Position position,
                                      PositionLookup* position_lookup);

void addPositionToLookup(Position position, Policy candidate,
                         PositionLookup* position_lookup);

void maybeSetCurrentBest(Policy& current_best, Policy& candidate,
                         bool maximizingPlayer);

bool willLose(Position position);
}  // namespace pentago