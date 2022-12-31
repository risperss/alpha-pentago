#include "negamax.h"

#include <cfloat>
#include <cstdint>

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

float Negamax::negamax(Position position, int depth, int color) {
  return negamax(position, depth, -FLT_MAX, FLT_MAX, 1);
}

TTEntry Negamax::transposition_table_lookup(Position position) {
  std::uint64_t hash = Hash(position);

  if (transposition_table.find(hash) != transposition_table.end()) {
    return transposition_table.find(hash)->second;
  } else {
    return empty;
  }
}

bool Negamax::is_valid(TTEntry tt_entry) { return tt_entry.depth != -999; }

void Negamax::transposition_table_store(Position position, TTEntry tt_entry) {
  // for (const std::uint64_t hash : PositionHashes(position)) {
  //   transposition_table[hash] = tt_entry;
  // }
  transposition_table[Hash(position)] = tt_entry;
}

PositionList Negamax::order_child_positions(Position position, MoveList moves) {
  PositionList child_positions;
  child_positions.reserve(moves.size());

  for (const Move move : moves) {
    child_positions.emplace_back(Position(position, move));
  }

  std::sort(child_positions.begin(), child_positions.end(),
            [this](const Position& a, const Position& b) {
              TTEntry tta = transposition_table_lookup(a);
              TTEntry ttb = transposition_table_lookup(b);
              return tta.value > ttb.value;
            });

  return child_positions;
}

float Negamax::negamax(Position position, int depth, float a, float b,
                       int color) {
  float a_orig = a;

  TTEntry tt_entry = transposition_table_lookup(position);

  if (is_valid(tt_entry) && tt_entry.depth >= depth) {
    if (tt_entry.flag == LookupFlag::EXACT) {
      return tt_entry.value;
    } else if (tt_entry.flag == LookupFlag::LOWERBOUND) {
      a = std::max(a, tt_entry.value);
    } else if (tt_entry.flag == LookupFlag::UPPERBOUND) {
      b = std::min(b, tt_entry.value);
    }
  }

  if (a >= b) {
    return tt_entry.value;
  }

  BoardResult board_result = position.GetBoard().ComputeBoardResult();
  nodes_visited++;

  if (board_result != BoardResult::UNDECIDED) {
    return kBoardResultValue.find(board_result)->second * color;
  } else if (depth == 0) {
    return heuristic_evaluator.value(position) * color;
  }

  MoveList legal_moves = position.GetBoard().SmartGenerateLegalMoves();
  float value = -FLT_MAX;

  for (const Move move : legal_moves) {
    value = std::max(
        value, -negamax(Position(position, move), depth - 1, -b, -a, -color));
    a = std::max(a, value);

    if (a >= b) {
      break;
    }
  }

  tt_entry.value = value;
  if (value <= a_orig) {
    tt_entry.flag = LookupFlag::UPPERBOUND;
  } else if (value >= b) {
    tt_entry.flag = LookupFlag::LOWERBOUND;
  } else {
    tt_entry.flag = LookupFlag::EXACT;
  }
  tt_entry.depth = depth;
  transposition_table_store(position, tt_entry);

  return value;
}

float Negamax::iddfs(Position position, int depth, int color) {
  float value = -FLT_MAX;

  for (int i = 1; i <= depth; i++) {
    value = std::max(value, negamax(position, i, color));
  }

  return value;
}

}  // namespace pentago
