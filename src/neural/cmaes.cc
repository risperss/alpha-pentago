#include "cmaes.h"

#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "neural/heuristic.h"
#include "neural/minimax.h"
#include "pentago/position.h"
#include "utils/lookup.h"

namespace pentago {

GameResult versus_game(Chromosome white_chromosome, Chromosome black_chromosome,
                       int search_depth) {
  HeuristicEvaluator* evaluator;
  HeuristicEvaluator white_evaluator = HeuristicEvaluator(white_chromosome);
  HeuristicEvaluator black_evaluator = HeuristicEvaluator(black_chromosome);

  PositionLookup* position_lookup = new PositionLookup;

  PentagoBoard board = PentagoBoard();
  Position starting = Position(board);
  PositionHistory history = PositionHistory(starting);

  int black_to_move = false;
  GameResult game_result = GameResult::UNDECIDED;

  while (game_result == GameResult::UNDECIDED) {
    int nodesVisited = 0;
    Policy policy;

    evaluator = black_to_move ? &black_evaluator : &white_evaluator;

    policy = minimax(history.Last(), search_depth, position_lookup,
                     &nodesVisited, *evaluator);

    history.Append(policy.move);

    black_to_move = !black_to_move;
    game_result = history.ComputeGameResult();

    position_lookup = clearedLookup(position_lookup, history.Last());
  }
  delete position_lookup;

  return game_result;
}

std::pair<int, int> match(Chromosome greece, Chromosome persia,
                          int games_per_side_per_match, int search_depth) {
  int greece_score = 0;
  int persia_score = 0;

  int* white_score = &greece_score;
  int* black_score = &persia_score;

  Chromosome* white_chromosome = &greece;
  Chromosome* black_chromosome = &persia;

  for (int i = 1; i <= 2 * games_per_side_per_match; i++) {
    GameResult game_result =
        versus_game(*white_chromosome, *black_chromosome, search_depth);

    if (game_result == GameResult::WHITE_WON) {
      *white_score += 3;
    } else if (game_result == GameResult::BLACK_WON) {
      *black_score += 3;
    } else {
      *white_score += 1;
      *black_score += 1;
    }

    std::swap(white_score, black_score);
    std::swap(white_chromosome, black_chromosome);
  }

  return std::pair<int, int>{greece_score, persia_score};
}

std::vector<float> fitnesses(std::vector<Chromosome> generation) {
  int games_per_side_per_match = 1;
  int lambda = generation.size();
  int games_played_per_genome = (lambda - 1) * 2 * games_per_side_per_match;
  int search_depth = 2;

  // will store loss percentage for each Chromosome
  std::vector<float> fitnesses = std::vector<float>(lambda, 0.0);

  for (int i = 0; i < lambda - 1; i++) {
    Chromosome greece = generation[i];
    for (int j = i + 1; j < lambda; j++) {
      Chromosome persia = generation[j];

      std::pair<int, int> scores =
          match(greece, persia, games_per_side_per_match, search_depth);

      fitnesses[i] += scores.first;
      fitnesses[j] += scores.second;
    }
  }

  for (int i = 0; i < lambda; i++) {
    fitnesses[i] = 1.0 / fitnesses[i];
  }

  return fitnesses;
}
}  // namespace pentago