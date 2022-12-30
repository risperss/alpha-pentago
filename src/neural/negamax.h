#include "neural/heuristic.h"
#include "pentago/board.h"
#include "pentago/position.h"

namespace pentago {

class Negamax {
 public:
  Negamax(HeuristicEvaluator heuristic_evaluator);

  // float negamax(Position position, int depth, int color);
  float negamax(Position position, int depth, float a, float b, int color);

  unsigned long long getNodesVisited() { return nodes_visited; }

 private:
  HeuristicEvaluator heuristic_evaluator;
  unsigned long long nodes_visited = 0;
};

}  // namespace pentago