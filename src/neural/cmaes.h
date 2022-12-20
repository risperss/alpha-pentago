#include <array>

#include "neural/heuristic.h"
#include "pentago/position.h"

namespace pentago {
PositionHistory randomSelfPlay();
float regret(HeuristicEvaluator* evaluator, PositionHistory history);
float fitness(std::array<float, kNumWeights> weights);
}  // namespace pentago