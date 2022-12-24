#include "game.h"

#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "neural/heuristic.h"
#include "neural/minimax.h"
#include "pentago/position.h"
#include "utils/lookup.h"

namespace pentago {
void selfPlay(int depth) {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  PositionLookup* lookup = new PositionLookup;
  HeuristicEvaluator evaluator = HeuristicEvaluator(kDefaultWeights);

  PentagoBoard board = PentagoBoard();
  Position starting = Position(board);
  PositionHistory history = PositionHistory(starting);

  unsigned long maxLookupSize = 0;
  unsigned long long totalNodesVisited = 0;
  long totalTimeTaken = 0;

  while (history.ComputeGameResult() == GameResult::UNDECIDED) {
    int nodesVisited = 0;

    auto t1 = high_resolution_clock::now();
    ReturnValue result =
        minimax(history.Last(), depth, lookup, &nodesVisited, evaluator);
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    unsigned long lookupSize = lookup->size() * sizeof(LookupItem);

    std::cout << "Ply Count:\t" << history.Last().GetPlyCount() << "\n";
    std::cout << "To Move:\t"
              << (history.Last().IsBlackToMove() ? "Black" : "White") << "\n";
    std::cout << "Value:\t\t" << result.value << "\n";
    std::cout << "Heuristic:\t" << evaluator.value(history.Last()) << std::endl;
    std::cout << "Move:\t\t" << result.move.as_string() << "\n";
    std::cout << "Nodes visited:\t" << (nodesVisited >> 10) << "k\n";
    std::cout << "Search time:\t" << ms_int.count() << " ms\n";
    std::cout << "Lookup size:\t" << (lookupSize >> 20) << " Mb\n";
    if (ms_int.count() != 0) {
      std::cout << "NPS:\t\t" << (nodesVisited / ms_int.count()) << " kN/s\n";
    }
    std::cout << history.Last().DebugString() << "\n";

    maxLookupSize = std::max(maxLookupSize, lookupSize);

    if (result.value == kMaxPositionValue ||
        result.value == kMinPositionValue) {
      std::string color = result.value == kMaxPositionValue ? "White" : "Black";
      std::cout << "Forced:\t" << color << " to win on ply "
                << static_cast<int>(result.plyCount) << "\n";
    }
    std::cout << "------------------------------" << std::endl;

    totalNodesVisited += nodesVisited;
    totalTimeTaken += ms_int.count();

    lookup = clearedLookup(lookup, history.Last());
    history.Append(result.move);
  }
  delete lookup;

  std::cout << "Ply Count:\t" << history.Last().GetPlyCount() << "\n";
  std::cout << "Result:\t\t"
            << resultString.find(history.ComputeGameResult())->second << "\n";
  std::cout << "Max RAM:\t" << (maxLookupSize >> 20) << " Mb\n";
  std::cout << "Search Depth:\t" << depth << std::endl;
  std::cout << "AVG NPS:\t" << (totalNodesVisited / totalTimeTaken)
            << " kN/s\n";
  std::cout << history.Last().DebugString() << "\n";
}

// Game class to be exposed via pybind11

Game::Game() {
  PentagoBoard starting_board = PentagoBoard();
  Position starting_position = Position(starting_board);
  position_history = PositionHistory(starting_position);

  position_lookup = new PositionLookup();
  heuristic_evaluator = HeuristicEvaluator(kDefaultWeights);
}

Game::~Game() { delete position_lookup; }

void Game::SetMaxSearchDepth(int depth) { max_search_depth = depth; }

MoveList Game::LegalMoves() {
  return position_history.Last().GetBoard().GenerateLegalMoves();
}

void Game::MakeMove(Move move) { position_history.Append(move); }

void Game::UnmakeMove() { position_history.Pop(); }

GameResult Game::ComputeResult() {
  return position_history.Last().ComputeGameResult();
}

Move Game::BestMove() {
  int nodes_visited = 0;
  ReturnValue return_value =
      minimax(position_history.Last(), max_search_depth, position_lookup,
              &nodes_visited, heuristic_evaluator);
  position_lookup = clearedLookup(position_lookup, position_history.Last());

  return return_value.move;
}

}  // namespace pentago