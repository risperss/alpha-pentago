#include "cmaes.h"

#include <cmath>
#include <iostream>
#include <random>
#include <vector>

#include "neural/heuristic.h"
#include "neural/minimax.h"
#include "pentago/position.h"

namespace pentago {

int versus_game(Chromosome white, Chromosome black, int search_depth) {
  HeuristicEvaluator* evaluator;
  HeuristicEvaluator white_evaluator = HeuristicEvaluator(white);
  HeuristicEvaluator black_evaluator = HeuristicEvaluator(black);

  PositionLookup* lookup = new PositionLookup;

  PentagoBoard board = PentagoBoard();
  Position starting = Position(board);
  PositionHistory history = PositionHistory(starting);

  int black_to_move = false;
  GameResult game_result = GameResult::UNDECIDED;

  while (game_result == GameResult::UNDECIDED) {
    int nodesVisited = 0;
    ReturnValue result;

    evaluator = black_to_move ? &black_evaluator : &white_evaluator;

    result =
        minimax(history.Last(), search_depth, lookup, &nodesVisited, evaluator);

    black_to_move = !black_to_move;
    history.Append(result.move);
    game_result = history.ComputeGameResult();

    delete lookup;
    lookup = new PositionLookup;
  }
  delete lookup;

  return terminalResultValue.find(game_result)->second;
}

std::pair<int, int> match(Chromosome greece, Chromosome persia,
                          int games_per_match, int search_depth) {
  int greece_wins = 0;
  int persia_wins = 0;

  bool greece_is_white = true;

  // TODO: cleanup this garbage
  for (int i = 1; i <= games_per_match; i++) {
    if (greece_is_white) {
      int result = versus_game(greece, persia, search_depth);
      if (result == 1) {
        greece_wins += 1;
      } else if (result == -1) {
        persia_wins += 1;
      }
    } else {
      int result = versus_game(persia, greece, search_depth);
      if (result == 1) {
        persia_wins += 1;
      } else if (result == -1) {
        greece_wins += 1;
      }
    }
    greece_is_white = !greece_is_white;
  }

  return std::pair<int, int>{greece_wins, persia_wins};
}

std::vector<float> fitnesses(std::vector<Chromosome> generation) {
  int games_per_match = 3;
  int lambda = generation.size();
  int games_played_per_genome = (lambda - 1) * games_per_match;
  int search_depth = 2;

  // will store loss percentage for each Chromosome
  std::vector<float> fitnesses = std::vector<float>(lambda, 0.0);

  for (int i = 0; i < lambda - 1; i++) {
    Chromosome greece = generation[i];
    for (int j = i + 1; j < lambda; j++) {
      Chromosome persia = generation[j];

      std::pair<int, int> wins =
          match(greece, persia, games_per_match, search_depth);

      fitnesses[i] += wins.first;
      fitnesses[j] += wins.second;
    }
  }

  for (int i = 0; i < lambda; i++) {
    fitnesses[i] = 1.0 / fitnesses[i];
  }

  return fitnesses;
}
}  // namespace pentago