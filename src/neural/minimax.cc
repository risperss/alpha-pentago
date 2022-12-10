#include "minimax.h"

#include <algorithm>
#include <unordered_map>

#include "neural/heuristic.h"
#include "pentago/position.h"
#include "utils/bitops.h"

namespace pen {
ReturnValue minimax(Position position, Move prevMove, int depth, int alpha,
                    int beta,
                    std::unordered_map<std::uint64_t, ReturnValue>* lookup) {
  GameResult result = GameResult::UNDECIDED;
  int plyCount = position.GetPlyCount();

  if (plyCount >= 9) {
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

    return ReturnValue{value, prevMove, plyCount};
  }

  MoveList* legalMoves = position.GetBoard().GenerateLegalMoves();

  if (position.IsBlackToMove()) {
    int minEval = INT32_MAX;
    Move move;
    ReturnValue returnValue;

    for (Move& m : *legalMoves) {
      Position p = Position(position, m);
      std::uint64_t hash = p.Hash();
      ReturnValue result;

      if (lookup->find(hash) != lookup->end()) {
        result = lookup->find(hash)->second;
      } else {
        result = minimax(p, m, depth - 1, alpha, beta, lookup);
        (*lookup)[hash] = result;
        (*lookup)[p.ReverseHash()] = result;
      }

      if (result.value < minEval) {
        returnValue.value = result.value;
        returnValue.move = m;
        minEval = result.value;
      } else if (returnValue.value == minEval) {
        if (minEval == MAX_POSITION_VALUE &&
            result.plyCount < returnValue.plyCount) {
          result = returnValue;
        } else if (minEval == -MAX_POSITION_VALUE &&
                   result.plyCount > returnValue.plyCount) {
          result = returnValue;
        }
      }
      beta = std::min(returnValue.value, beta);

      if (beta <= alpha) {
        break;
      }
    }
    delete legalMoves;
    return returnValue;
  } else {
    int maxEval = -INT32_MAX;
    ReturnValue returnValue;

    for (Move& m : *legalMoves) {
      Position p = Position(position, m);
      std::uint64_t hash = p.Hash();
      ReturnValue result;

      if (lookup->find(hash) != lookup->end()) {
        result = lookup->find(hash)->second;
      } else {
        result = minimax(p, m, depth - 1, alpha, beta, lookup);
        (*lookup)[hash] = result;
        (*lookup)[p.ReverseHash()] = result;
      }

      if (result.value > maxEval) {
        returnValue.value = result.value;
        returnValue.move = m;
        maxEval = result.value;
      } else if (returnValue.value == maxEval) {
        if (maxEval == -MAX_POSITION_VALUE &&
            result.plyCount > returnValue.plyCount) {
          result = returnValue;
        } else if (maxEval == MAX_POSITION_VALUE &&
                   result.plyCount < returnValue.plyCount) {
          result = returnValue;
        }
      }
      alpha = std::max(result.value, alpha);

      if (beta <= alpha) {
        break;
      }
    }
    delete legalMoves;
    return returnValue;
  }
}

ReturnValue minimax(Position position,
                    std::unordered_map<std::uint64_t, ReturnValue>* lookup) {
  return minimax(position, Move(std::uint16_t(0)), DEPTH, -INT32_MAX, INT32_MAX,
                 lookup);
}
}  // namespace pen
