#include "minimax.h"

#include <algorithm>
#include <unordered_map>

#include "neural/heuristic.h"
#include "pentago/position.h"
#include "utils/bitops.h"

namespace pen {
ReturnValue minimax(Position position, Move prevMove, int depth, int alpha,
                    int beta, bool maximizingPlayer, PositionLookup* lookup) {
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

  if (maximizingPlayer) {
    ReturnValue currentBest;
    currentBest.value = -INT32_MAX;  // maxValue

    for (Move& m : legalMoves) {
      ReturnValue candidate;

      Position p = Position(position, m);
      std::uint64_t hash = p.Hash();

      if (lookup->find(hash) != lookup->end()) {
        candidate = lookup->find(hash)->second;
      } else {
        candidate =
            minimax(p, m, depth - 1, alpha, beta, !maximizingPlayer, lookup);
        (*lookup)[hash] = candidate;
        (*lookup)[p.ReverseHash()] = candidate;
      }

      if (candidate.value > currentBest.value) {
        currentBest.value = candidate.value;
        currentBest.move = m;
        currentBest.plyCount = candidate.plyCount;
      } else if (candidate.value == currentBest.value) {
        // Losing, take longest path
        if (candidate.value == -MAX_POSITION_VALUE &&
            candidate.plyCount > currentBest.plyCount) {
          currentBest = candidate;
          // Winning, take shortest path
        } else if (candidate.value == MAX_POSITION_VALUE &&
                   candidate.plyCount < currentBest.plyCount) {
          currentBest = candidate;
        }
      }

      if (currentBest.value >= beta) {
        break;
      }
      alpha = std::max(currentBest.value, alpha);
    }
    return currentBest;
  } else {
    ReturnValue currentBest;
    currentBest.value = INT32_MAX;  // minValue

    for (Move& m : legalMoves) {
      ReturnValue candidate;

      Position p = Position(position, m);
      std::uint64_t hash = p.Hash();

      if (lookup->find(hash) != lookup->end()) {
        candidate = lookup->find(hash)->second;
      } else {
        candidate =
            minimax(p, m, depth - 1, alpha, beta, !maximizingPlayer, lookup);
        (*lookup)[hash] = candidate;
        (*lookup)[p.ReverseHash()] = candidate;
      }

      if (candidate.value < currentBest.value) {
        currentBest.value = candidate.value;
        currentBest.move = m;
        currentBest.plyCount = candidate.plyCount;
      } else if (candidate.value == currentBest.value) {
        // Losing, take longest path
        if (candidate.value == MAX_POSITION_VALUE &&
            candidate.plyCount > currentBest.plyCount) {
          currentBest = candidate;
          // Winning, take shortest path
        } else if (candidate.value == -MAX_POSITION_VALUE &&
                   candidate.plyCount < currentBest.plyCount) {
          currentBest = candidate;
        }
      }

      if (currentBest.value <= alpha) {
        break;
      }
      beta = std::min(currentBest.value, beta);
    }
    return currentBest;
  }
}

ReturnValue minimax(Position position, PositionLookup* lookup) {
  return minimax(position, Move(std::uint16_t(0)), DEPTH, -INT32_MAX, INT32_MAX,
                 !position.IsBlackToMove(), lookup);
}
}  // namespace pen
