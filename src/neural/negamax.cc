#include "negamax.h"

#include <algorithm>
#include <cfloat>
#include <chrono>
#include <cstdint>
#include <random>

#include "neural/heuristic.h"
#include "utils/hashcat.h"

namespace pentago {

Negamax::Negamax() {
  heuristic_evaluator = HeuristicEvaluator(kDefaultGenome);
  transposition_table = TT();
}

Negamax::Negamax(Genome genome) {
  heuristic_evaluator = HeuristicEvaluator(genome);
  transposition_table = TT();
}

NReturn Negamax::best(Position position, int depth) {
  nodes_visited = 0;
  // TODO: implement smart clearing of transposition table.
  transposition_table = TT();
  return negamax(position, depth, -FLT_MAX, FLT_MAX);
}

TTEntry Negamax::transposition_table_lookup(Position position) {
  std::uint64_t hash = Hash(position);

  if (transposition_table.find(hash) != transposition_table.end()) {
    return transposition_table.find(hash)->second;
  } else {
    return kEmptyEntry;
  }
}

bool Negamax::is_valid(TTEntry tt_entry) { return tt_entry != kEmptyEntry; }

void Negamax::transposition_table_store(Position position, TTEntry tt_entry) {
  for (const std::uint64_t hash : PositionHashes(position)) {
    transposition_table[hash] = tt_entry;
  }
}

void Negamax::order_moves(MoveList& legal_moves) {
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  std::shuffle(std::begin(legal_moves), std::end(legal_moves),
               std::default_random_engine(seed));
}

NReturn Negamax::negamax(Position position, int depth, float a, float b) {
  nodes_visited++;
  float a_orig = a;

  TTEntry tt_entry = transposition_table_lookup(position);

  if (is_valid(tt_entry)) {
    if (tt_entry.flag == LookupFlag::EXACT) {
      return {tt_entry.value, tt_entry.move};
    } else if (tt_entry.flag == LookupFlag::LOWERBOUND) {
      a = std::max(a, tt_entry.value);
    } else if (tt_entry.flag == LookupFlag::UPPERBOUND) {
      b = std::min(b, tt_entry.value);
    }

    if (a >= b) {
      return {tt_entry.value, tt_entry.move};
    }
  }

  BoardResult board_result = position.GetBoard().ComputeBoardResult();

  if (board_result != BoardResult::UNDECIDED) {
    return {kBoardResultValue.find(board_result)->second + depth, kNullMove};
  } else if (depth == 0) {
    return {heuristic_evaluator.value(position), kNullMove};
  }

  MoveList legal_moves = position.GetBoard().SmartGenerateLegalMoves();
  order_moves(legal_moves);

  NReturn n_return = {-FLT_MAX, kNullMove};

  for (Move move : legal_moves) {
    NReturn candidate = -negamax(Position(position, move), depth - 1, -b, -a);

    if (candidate > n_return) {
      n_return.value = candidate.value;
      n_return.move = move;
    }

    a = std::max(a, n_return.value);

    if (a >= b) {
      break;
    }
  }

  tt_entry.value = n_return.value;
  if (n_return.value <= a_orig) {
    tt_entry.flag = LookupFlag::UPPERBOUND;
  } else if (n_return.value >= b) {
    tt_entry.flag = LookupFlag::LOWERBOUND;
  } else {
    tt_entry.flag = LookupFlag::EXACT;
  }
  tt_entry.move = n_return.move;
  transposition_table_store(position, tt_entry);

  return n_return;
}

}  // namespace pentago
