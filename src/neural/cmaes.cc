#include "cmaes.h"

#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "neural/heuristic.h"
#include "neural/minimax.h"
#include "pentago/position.h"

namespace pentago {

PositionHistory randomSelfPlay() {
  PentagoBoard board = PentagoBoard();
  Position starting = Position(board);
  PositionHistory history = PositionHistory(starting);

  while (history.ComputeGameResult() == GameResult::UNDECIDED) {
    MoveList legalMoves = history.Last().GetBoard().GenerateLegalMoves();

    // randomize
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    shuffle(legalMoves.begin(), legalMoves.end(),
            std::default_random_engine(seed));

    history.Append(legalMoves.front());
  }

  return history;
}

float regret(HeuristicEvaluator* evaluator, Position position) {
  PositionLookup lookup;
  int nodesVisited = 0;

  float heuristicValue = evaluator->value(position);
  ReturnValue returnValue =
      minimax(position, INT_MAX, &lookup, &nodesVisited, evaluator);
  float regret = std::abs(returnValue.value - heuristicValue);

  return regret;
}

float fitness(std::array<float, kNumWeights> weights) {
  HeuristicEvaluator* evaluator = new HeuristicEvaluator(weights);
  float sumRegret = 0.0;
  int sampleCount = 5;

  int i = 1;
  while (i <= sampleCount) {
    PositionHistory history = randomSelfPlay();

    if (history.Last().GetPlyCount() >= 33) {
      sumRegret +=
          regret(evaluator, history.GetPositionAt(history.GetLength() - 4));
      i++;
    }
  }
  delete evaluator;

  float averageRegret = sumRegret / sampleCount;

  return averageRegret;
}
}  // namespace pentago