#include "minimax.h"

#include <algorithm>
#include <unordered_map>

#include "neural/heuristic.h"
#include "pentago/position.h"
#include "utils/bitops.h"

namespace pen {
ReturnValue minimax(Position position, Move prevMove, int depth, int alpha,
                    int beta, PositionLookup* lookup) {
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

  MoveList legalMoves = position.GetBoard().GenerateLegalMoves();

  if (position.IsBlackToMove()) {
    ReturnValue returnValue;
    returnValue.value = INT32_MAX;  // minValue

    for (Move& m : legalMoves) {
      ReturnValue result;

      Position p = Position(position, m);
      std::uint64_t hash = p.Hash();

      if (lookup->find(hash) != lookup->end()) {
        result = lookup->find(hash)->second;
      } else {
        result = minimax(p, m, depth - 1, alpha, beta, lookup);
        (*lookup)[hash] = result;
        (*lookup)[p.ReverseHash()] = result;
      }

      if (result.value < returnValue.value) {
        returnValue.value = result.value;
        returnValue.move = m;
        returnValue.plyCount = result.plyCount;
      } else if (result.value == returnValue.value) {
        // Losing, take longest path
        if (result.value == MAX_POSITION_VALUE &&
            result.plyCount > returnValue.plyCount) {
          returnValue = result;
          // Winning, take shortest path
        } else if (result.value == -MAX_POSITION_VALUE &&
                   result.plyCount < returnValue.plyCount) {
          returnValue = result;
        }
      }
      beta = std::min(result.value, beta);

      if (beta <= alpha) {
        break;
      }
    }
    return returnValue;
  } else {
    ReturnValue returnValue;
    returnValue.value = -INT32_MAX;  // maxValue

    for (Move& m : legalMoves) {
      ReturnValue result;

      Position p = Position(position, m);
      std::uint64_t hash = p.Hash();

      if (lookup->find(hash) != lookup->end()) {
        result = lookup->find(hash)->second;
      } else {
        result = minimax(p, m, depth - 1, alpha, beta, lookup);
        (*lookup)[hash] = result;
        (*lookup)[p.ReverseHash()] = result;
      }

      if (result.value > returnValue.value) {
        returnValue.value = result.value;
        returnValue.move = m;
        returnValue.plyCount = result.plyCount;
      } else if (result.value == returnValue.value) {
        // Losing, take longest path
        if (result.value == -MAX_POSITION_VALUE &&
            result.plyCount > returnValue.plyCount) {
          returnValue = result;
          // Winning, take shortest path
        } else if (result.value == MAX_POSITION_VALUE &&
                   result.plyCount < returnValue.plyCount) {
          returnValue = result;
        }
      }
      alpha = std::max(result.value, alpha);

      if (beta <= alpha) {
        break;
      }
    }
    return returnValue;
  }
}

ReturnValue minimax(Position position, PositionLookup* lookup) {
  return minimax(position, Move(std::uint16_t(0)), DEPTH, -INT32_MAX, INT32_MAX,
                 lookup);
}
}  // namespace pen
