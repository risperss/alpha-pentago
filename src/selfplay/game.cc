#include "game.h"

#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "neural/heuristic.h"
#include "neural/minimax.h"
#include "pentago/position.h"
#include "utils/bitops.h"
#include "utils/lookup.h"

namespace pentago {
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
  Policy policy = minimax(position_history.Last(), max_search_depth,
                          position_lookup, &nodes_visited, heuristic_evaluator);
  position_lookup = clearedLookup(position_lookup, position_history.Last());

  return policy.move;
}

std::string Game::DebugString() {
  return position_history.Last().DebugString();
}
}  // namespace pentago