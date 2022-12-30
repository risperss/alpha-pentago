#include "negamax.h"

#include <cfloat>

namespace pentago {

Negamax::Negamax(HeuristicEvaluator heuristic_evaluator) {
  this->heuristic_evaluator = heuristic_evaluator;
}

// 0 optimizations
// float Negamax::negamax(Position position, int depth, int color) {
//   BoardResult board_result = position.GetBoard().ComputeBoardResult();
//   nodes_visited++;

//   if (depth == 0) {
//     return heuristic_evaluator.value(position) * color;
//   } else if (board_result != BoardResult::UNDECIDED) {
//     return kBoardResultValue.find(board_result)->second * color;
//   }

//   MoveList legal_moves = position.GetBoard().GenerateLegalMoves();
//   float value = -FLT_MAX;

//   for (const Move move : legal_moves) {
//     value =
//         std::max(value, negamax(Position(position, move), depth - 1,
//         -color));
//   }

//   return value;
// }

// Just smart move generation (huge difference... wow)
// float Negamax::negamax(Position position, int depth, int color) {
//   BoardResult board_result = position.GetBoard().ComputeBoardResult();
//   nodes_visited++;

//   if (depth == 0) {
//     return heuristic_evaluator.value(position) * color;
//   } else if (board_result != BoardResult::UNDECIDED) {
//     return kBoardResultValue.find(board_result)->second * color;
//   }

//   MoveList legal_moves = position.GetBoard().SmartGenerateLegalMoves();
//   float value = -FLT_MAX;

//   for (const Move move : legal_moves) {
//     value =
//         std::max(value, negamax(Position(position, move), depth - 1,
//         -color));
//   }

//   return value;
// }

// Just alpha-beta pruning
float Negamax::negamax(Position position, int depth, float a, float b,
                       int color) {
  BoardResult board_result = position.GetBoard().ComputeBoardResult();
  nodes_visited++;

  if (depth == 0) {
    return heuristic_evaluator.value(position) * color;
  } else if (board_result != BoardResult::UNDECIDED) {
    return kBoardResultValue.find(board_result)->second * color;
  }

  MoveList legal_moves = position.GetBoard().SmartGenerateLegalMoves();
  float value = -FLT_MAX;

  for (const Move move : legal_moves) {
    value =
        std::max(value, negamax(Position(position, move), depth - 1, -color));
  }

  return value;
}

}  // namespace pentago
