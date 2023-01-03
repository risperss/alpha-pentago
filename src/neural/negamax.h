#include <algorithm>
#include <cstdint>
#include <unordered_map>

#include "neural/heuristic.h"
#include "pentago/board.h"
#include "pentago/position.h"

#pragma once

namespace pentago {

enum class LookupFlag : uint8_t { LOWERBOUND, EXACT, UPPERBOUND };

struct TTEntry {
  float value;
  Move move;
  LookupFlag flag;

  bool operator==(const TTEntry& rhs) const {
    return value == rhs.value && move == rhs.move && flag == rhs.flag;
  }
  bool operator!=(const TTEntry& rhs) const {
    return value != rhs.value || move != rhs.move || flag != rhs.flag;
  }
};

struct NReturn {
  float value;
  Move move;

  bool operator<(const NReturn& rhs) const { return value < rhs.value; }
  bool operator>(const NReturn& rhs) const { return value > rhs.value; }
  NReturn operator-() const { return NReturn{-value, move}; }
};

using TT = std::unordered_map<uint64_t, TTEntry>;

static const Move kNullMove = Move(std::uint16_t(0xFFFF));

const TTEntry kEmptyEntry =
    TTEntry{kMinBoardValue, kNullMove, LookupFlag::EXACT};

static const Genome kDefaultGenome = {0.67168461,  0.10707687, 0.06840958,
                                      -0.29951185, 0.21427413, 0.33170923,
                                      0.68111354,  -0.04503335};

class Negamax {
 public:
  Negamax();
  Negamax(Genome genome);

  NReturn best(Position position, int depth);

  unsigned long long getNodesVisited() { return nodes_visited; }

 private:
  TTEntry transposition_table_lookup(Position position);

  bool is_valid(TTEntry entry);

  void transposition_table_store(Position position, TTEntry tt_entry);

  void order_moves(MoveList& legal_moves);

  NReturn negamax(Position position, int depth, float a, float b);

  HeuristicEvaluator heuristic_evaluator;

  unsigned long long nodes_visited = 0;

  TT transposition_table;
};

}  // namespace pentago