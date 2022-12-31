#include <cstdint>
#include <unordered_map>

#include "pentago/position.h"

namespace pentago {
class Game {
 private:
  int max_search_depth = 4;
  PositionLookup* position_lookup;
  PositionHistory position_history;
  HeuristicEvaluator heuristic_evaluator;

 public:
  Game();
  ~Game();

  void SetMaxSearchDepth(int max_search_depth);

  MoveList LegalMoves();

  void MakeMove(Move move);
  void UnmakeMove();

  GameResult ComputeResult();

  Move BestMove();

  std::string DebugString();
};
}  // namespace pentago