#include "minimax.h"

#include <algorithm>
#include <unordered_map>

#include "neural/heuristic.h"
#include "pentago/position.h"
#include "utils/bitops.h"

namespace pen {
std::pair<Move, int> minimax(Position position, Move prevMove, int depth,
                             int alpha, int beta,
                             std::unordered_map<std::uint64_t, int>* lookup) {
  GameResult result = GameResult::UNDECIDED;

  if (position.GetPlyCount() >= 9) {
    result = position.ComputeGameResult();
  }

  if (depth == 0 || result != GameResult::UNDECIDED) {
    int value;

    if (result == GameResult::WHITE_WON) {
      value = MAX_POSITION_VALUE;
    } else if (result == GameResult::DRAW) {
      value = 0;
    } else if (result == GameResult::BLACK_WON) {
      value = -MAX_POSITION_VALUE;
    } else {
      value = heuristic_value(position);
    }

    return std::pair<Move, int>{prevMove, value};
  }

  MoveList* legalMoves = position.GetBoard().GenerateLegalMoves();

  if (position.IsBlackToMove()) {
    int minEval = INT32_MAX;
    Move move;

    for (Move& m : *legalMoves) {
      Position p = Position(position, m);
      std::uint64_t hash = p.Hash();
      int eval;

      if (lookup->find(hash) != lookup->end()) {
        eval = lookup->find(hash)->second;
      } else {
        eval = std::get<int>(minimax(p, m, depth - 1, alpha, beta, lookup));
        (*lookup)[hash] = eval;
        (*lookup)[p.ReverseHash()] = eval;
      }

      if (eval < minEval) {
        minEval = eval;
        move = m;
      }
      beta = std::min(eval, beta);

      if (beta <= alpha) {
        break;
      }
    }
    delete legalMoves;
    return std::pair<Move, int>(move, minEval);
  } else {
    int maxEval = -INT32_MAX;
    Move move;

    for (Move& m : *legalMoves) {
      Position p = Position(position, m);
      std::uint64_t hash = p.Hash();
      int eval;

      if (lookup->find(hash) != lookup->end()) {
        eval = lookup->find(hash)->second;
      } else {
        eval = std::get<int>(minimax(p, m, depth - 1, alpha, beta, lookup));
        (*lookup)[hash] = eval;
        (*lookup)[p.ReverseHash()] = eval;
      }

      if (eval > maxEval) {
        maxEval = eval;
        move = m;
      }
      alpha = std::max(eval, alpha);

      if (beta <= alpha) {
        break;
      }
    }
    delete legalMoves;
    return std::pair<Move, int>(move, maxEval);
  }
}

std::pair<Move, int> value(Position position,
                           std::unordered_map<std::uint64_t, int>* lookup) {
  return minimax(position, Move(std::uint16_t(0)), DEPTH, -INT32_MAX, INT32_MAX,
                 lookup);
}
}  // namespace pen
