#include <cstdint>
#include <unordered_map>

#include "neural/heuristic.h"
#include "pentago/board.h"
#include "pentago/position.h"

namespace pentago {

enum class LookupFlag : uint8_t { LOWERBOUND, EXACT, UPPERBOUND };

struct TTEntry {
  float value;
  int depth;
  LookupFlag flag;
};

using TT = std::unordered_map<uint64_t, TTEntry>;

const TTEntry empty = TTEntry{0, -999, LookupFlag::EXACT};

class Negamax {
 public:
  Negamax(HeuristicEvaluator heuristic_evaluator);

  float negamax(Position position, int depth, int color);

  unsigned long long getNodesVisited() { return nodes_visited; }

 private:
  TTEntry transposition_table_lookup(Position position);
  bool is_valid(TTEntry entry);
  void transposition_table_store(Position position, TTEntry tt_entry);

  float negamax(Position position, int depth, float a, float b, int color);

  HeuristicEvaluator heuristic_evaluator;
  unsigned long long nodes_visited = 0;

  TT transposition_table;
};

}  // namespace pentago