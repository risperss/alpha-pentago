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
Policy minimax(Position position, Move prev_move, int depth, float alpha,
               float beta, bool maximizingPlayer,
               PositionLookup* position_lookup, int* nodesVisited,
               HeuristicEvaluator evaluator) {
  GameResult game_result = GameResult::UNDECIDED;
  const short ply_count = position.GetPlyCount();
  (*nodesVisited)++;

  if (ply_count >= 9) {
    game_result = position.ComputeGameResult();
  }

  if (game_result != GameResult::UNDECIDED) {
    return Policy{kTerminalResultValue.find(game_result)->second, ply_count,
                  prev_move};
  } else if (depth == 0) {
    return Policy{evaluator.value(position), ply_count, prev_move};
  }

  MoveList legal_moves = position.GetBoard().SmartGenerateLegalMoves();

  // Randomize moves
  unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
  shuffle(legal_moves.begin(), legal_moves.end(),
          std::default_random_engine(seed));

  // Search function
  if (maximizingPlayer) {
    Policy current_best;
    current_best.value = kNegativeInfinity;  // maxValue
    bool at_least_one = false;

    for (Move& move : legal_moves) {
      Policy candidate;
      candidate.move = move;

      Position candidate_position = Position(position, move);

      if (ply_count >= 8 && at_least_one && willLose(candidate_position)) {
        continue;
      }

      std::pair<Value, bool> looked_up_position =
          lookupPosition(candidate_position, position_lookup);

      if (looked_up_position.second) {
        candidate.value = looked_up_position.first.value;
        candidate.ply_count = looked_up_position.first.ply_count;
      } else {
        Policy result = minimax(candidate_position, move, depth - 1, alpha,
                                beta, !maximizingPlayer, position_lookup,
                                nodesVisited, evaluator);
        candidate.value = result.value;
        candidate.ply_count = result.ply_count;

        addPositionToLookup(candidate_position, result, position_lookup);
      }
      maybeSetCurrentBest(current_best, candidate, maximizingPlayer);

      if (current_best.value >= beta) {
        break;
      }
      alpha = std::max(current_best.value, alpha);
      at_least_one = true;
    }
    return current_best;
  } else {
    Policy current_best;
    current_best.value = kPositiveInfinity;  // minValue
    bool at_least_one = false;

    for (Move& move : legal_moves) {
      Policy candidate;
      candidate.move = move;

      Position candidate_position = Position(position, move);

      if (ply_count >= 8 && at_least_one && willLose(candidate_position)) {
        continue;
      }

      std::pair<Value, bool> looked_up_position =
          lookupPosition(candidate_position, position_lookup);

      if (looked_up_position.second) {
        candidate.value = looked_up_position.first.value;
        candidate.ply_count = looked_up_position.first.ply_count;
      } else {
        Policy result = minimax(candidate_position, move, depth - 1, alpha,
                                beta, !maximizingPlayer, position_lookup,
                                nodesVisited, evaluator);
        candidate.value = result.value;
        candidate.ply_count = result.ply_count;

        addPositionToLookup(candidate_position, result, position_lookup);
      }
      maybeSetCurrentBest(current_best, candidate, maximizingPlayer);

      if (current_best.value <= alpha) {
        break;
      }
      beta = std::min(current_best.value, beta);
      at_least_one = true;
    }
    return current_best;
  }
}

Policy minimax(Position position, int depth, PositionLookup* position_lookup,
               int* nodesVisited, HeuristicEvaluator evaluator) {
  return minimax(position, Move(std::uint16_t(0)), depth, kNegativeInfinity,
                 kPositiveInfinity, !position.IsBlackToMove(), position_lookup,
                 nodesVisited, evaluator);
}

std::pair<Value, bool> lookupPosition(Position position,
                                      PositionLookup* position_lookup) {
  Value value;

  std::uint64_t our_pieces = position.GetBoard().our_pieces().as_int();
  std::uint64_t their_pieces = position.GetBoard().their_pieces().as_int();

  HashList hashes = PositionHashes(our_pieces, their_pieces);

  bool found_position = false;

  for (std::uint64_t& hash : hashes) {
    if (position_lookup->find(hash) != position_lookup->end()) {
      value = position_lookup->find(hash)->second;
      found_position = true;
      break;
    }
  }

  return {value, found_position};
}

void addPositionToLookup(Position position, Policy candidate,
                         PositionLookup* position_lookup) {
  Value item = Value{candidate.value, candidate.ply_count};
  (*position_lookup)[HashCat(position.GetBoard().our_pieces().as_int(),
                             position.GetBoard().their_pieces().as_int())] =
      item;
}

void maybeSetCurrentBest(Policy& current_best, Policy& candidate,
                         bool maximizingPlayer) {
  if (maximizingPlayer) {
    if (candidate.value > current_best.value) {
      current_best = candidate;
    } else if (candidate.value == current_best.value) {
      // Losing, take longest path
      if (candidate.value == kMinPositionValue &&
          candidate.ply_count > current_best.ply_count) {
        current_best = candidate;
        // Winning, take shortest path
      } else if (candidate.value == kMaxPositionValue &&
                 candidate.ply_count < current_best.ply_count) {
        current_best = candidate;
      }
    }
  } else {
    if (candidate.value < current_best.value) {
      current_best = candidate;
    } else if (candidate.value == current_best.value) {
      // Losing, take longest path
      if (candidate.value == kMaxPositionValue &&
          candidate.ply_count > current_best.ply_count) {
        current_best = candidate;
        // Winning, take shortest path
      } else if (candidate.value == kMinPositionValue &&
                 candidate.ply_count < current_best.ply_count) {
        current_best = candidate;
      }
    }
  }
}

bool willLose(Position position) {
  // Position after we have made move... their pieces now our pieces
  std::uint64_t our_pieces = position.GetBoard().our_pieces().as_int();
  std::uint64_t their_pieces = position.GetBoard().their_pieces().as_int();

  for (std::uint64_t mask : kWinningMasks) {
    if (count_few(our_pieces & mask) == 4 &&
        count_few(their_pieces & mask) == 0) {
      return true;
    }
  }

  return false;
}
}  // namespace pentago
