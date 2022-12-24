#include "minimax.h"

#include <algorithm>
#include <array>
#include <chrono>
#include <iostream>
#include <random>
#include <unordered_map>

#include "neural/heuristic.h"
#include "pentago/position.h"
#include "utils/hashcat.h"

namespace pentago {
ReturnValue minimax(Position position, Move prevMove, int depth, float alpha,
                    float beta, bool maximizingPlayer, PositionLookup* lookup,
                    int* nodesVisited, HeuristicEvaluator evaluator) {
  GameResult result = GameResult::UNDECIDED;
  std::uint8_t plyCount = std::uint8_t(position.GetPlyCount());
  (*nodesVisited)++;

  if (plyCount >= 9) {
    result = position.ComputeGameResult();
  }

  // Value function
  if (depth == 0 || result != GameResult::UNDECIDED) {
    float value;

    if (depth == 0) {
      value = evaluator.value(position);
    } else {
      value = terminalResultValue.find(result)->second;
    }

    return ReturnValue{value, prevMove, plyCount};
  }

  MoveList legalMoves = position.GetBoard().SmartGenerateLegalMoves();

  // Randomize moves
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle(legalMoves.begin(), legalMoves.end(),
          std::default_random_engine(seed));

  // Search function
  if (maximizingPlayer) {
    ReturnValue currentBest;
    currentBest.value = kNegativeInfinity;  // maxValue

    for (Move& m : legalMoves) {
      // SAME ABOVE AS BELOW
      ReturnValue candidate;
      candidate.move = m;

      Position candidatePosition = Position(position, m);

      std::uint64_t our_pieces =
          candidatePosition.GetBoard().our_pieces().as_int();
      std::uint64_t their_pieces =
          candidatePosition.GetBoard().their_pieces().as_int();

      std::array<std::uint64_t, 2> hashes =
          PositionHashes(our_pieces, their_pieces);

      bool foundMatchingHash = false;

      for (std::uint64_t& hash : hashes) {
        if (lookup->find(hash) != lookup->end()) {
          LookupItem item = lookup->find(hash)->second;
          candidate.value = item.value;
          candidate.plyCount = item.plyCount;

          (*nodesVisited)++;
          foundMatchingHash = true;
          break;
        }
      }

      if (!foundMatchingHash) {
        ReturnValue result =
            minimax(candidatePosition, m, depth - 1, alpha, beta,
                    !maximizingPlayer, lookup, nodesVisited, evaluator);
        candidate.value = result.value;
        candidate.plyCount = result.plyCount;

        LookupItem item = LookupItem{candidate.value, candidate.plyCount};
        (*lookup)[hashes[0]] = item;
      }
      /////////////////////

      if (candidate.value > currentBest.value) {
        currentBest = candidate;
      } else if (candidate.value == currentBest.value) {
        // Losing, take longest path
        if (candidate.value == kMinPositionValue &&
            candidate.plyCount > currentBest.plyCount) {
          currentBest = candidate;
          // Winning, take shortest path
        } else if (candidate.value == kMaxPositionValue &&
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
    currentBest.value = kPositiveInfinity;  // minValue

    for (Move& m : legalMoves) {
      // SAME ABOVE AS BELOW
      ReturnValue candidate;
      candidate.move = m;

      Position candidatePosition = Position(position, m);

      std::uint64_t our_pieces =
          candidatePosition.GetBoard().our_pieces().as_int();
      std::uint64_t their_pieces =
          candidatePosition.GetBoard().their_pieces().as_int();

      std::array<std::uint64_t, 2> hashes =
          PositionHashes(our_pieces, their_pieces);

      bool foundMatchingHash = false;

      for (std::uint64_t& hash : hashes) {
        if (lookup->find(hash) != lookup->end()) {
          LookupItem item = lookup->find(hash)->second;
          candidate.value = item.value;
          candidate.plyCount = item.plyCount;

          (*nodesVisited)++;
          foundMatchingHash = true;
          break;
        }
      }

      if (!foundMatchingHash) {
        ReturnValue result =
            minimax(candidatePosition, m, depth - 1, alpha, beta,
                    !maximizingPlayer, lookup, nodesVisited, evaluator);
        candidate.value = result.value;
        candidate.plyCount = result.plyCount;

        LookupItem item = LookupItem{candidate.value, candidate.plyCount};
        (*lookup)[hashes[0]] = item;
      }
      /////////////////////

      if (candidate.value < currentBest.value) {
        currentBest = candidate;
      } else if (candidate.value == currentBest.value) {
        // Losing, take longest path
        if (candidate.value == kMaxPositionValue &&
            candidate.plyCount > currentBest.plyCount) {
          currentBest = candidate;
          // Winning, take shortest path
        } else if (candidate.value == kMinPositionValue &&
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

ReturnValue minimax(Position position, int depth, PositionLookup* lookup,
                    int* nodesVisited, HeuristicEvaluator evaluator) {
  return minimax(position, Move(std::uint16_t(0)), depth, kNegativeInfinity,
                 kPositiveInfinity, !position.IsBlackToMove(), lookup,
                 nodesVisited, evaluator);
}
}  // namespace pentago
