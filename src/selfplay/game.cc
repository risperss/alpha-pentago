#include "game.h"

#include <iostream>

#include "neural/minimax.h"
#include "pentago/position.h"

namespace pen {
PositionLookup* smartClearedLookup(PositionLookup* lookup) {
  PositionLookup* newLookup = new PositionLookup;

  for (auto& [hash, returnValue] : *lookup) {
    if (returnValue.value == MAX_POSITION_VALUE ||
        returnValue.value == -MAX_POSITION_VALUE) {
      (*newLookup)[hash] = returnValue;
    }
  }

  delete lookup;
  return newLookup;
}

void selfPlay() {
  PositionLookup* lookup = new PositionLookup;

  pen::PentagoBoard board = pen::PentagoBoard();
  pen::Position starting = pen::Position(board);
  pen::PositionHistory history = pen::PositionHistory(starting);

  while (history.ComputeGameResult() == pen::GameResult::UNDECIDED) {
    ReturnValue result = pen::minimax(history.Last(), lookup);

    std::cout << "Ply Count: " << history.Last().GetPlyCount() << "\n";
    std::cout << "To Move: "
              << (history.Last().IsBlackToMove() ? "Black" : "White") << "\n";
    std::cout << "Value: " << result.value << "\n";
    std::cout << "Move to be made: " << result.move.as_string() << "\n";
    std::cout << history.Last().DebugString() << "\n";
    std::cout << "------------------------------" << std::endl;

    lookup = smartClearedLookup(lookup);
    history.Append(result.move);
  }
  delete lookup;

  std::cout << history.Last().DebugString() << "\n";
  std::cout << pen::resultString.find(history.ComputeGameResult())->second
            << "\n";
  std::cout << "Depth: " << pen::DEPTH << std::endl;
}

// void vsHuman() {
//   std::unordered_map<std::uint64_t, int>* lookup =
//       new std::unordered_map<std::uint64_t, int>;

//   pen::PentagoBoard board = pen::PentagoBoard();
//   pen::Position starting = pen::Position(board);
//   pen::PositionHistory history = pen::PositionHistory(starting);

//   bool humanMove = true;

//   while (history.ComputeGameResult() == pen::GameResult::UNDECIDED) {
//     pen::Move move;

//     if (humanMove) {
//       std::cout << history.Last().DebugString() << std::endl;

//       std::string moveString;

//       std::cout << "Enter a move: ";
//       std::cin >> moveString;

//       move = pen::Move(moveString);
//     } else {
//       std::pair<pen::Move, int> result = pen::minimax(history.Last(),
//       lookup);

//       move = std::get<pen::Move>(result);

//       std::cout << "CPU Played: " << move.as_string() << std::endl;
//     }

//     lookup = smartClearedLookup(lookup);
//     history.Append(move);
//     humanMove = !humanMove;
//   }

//   std::cout << history.Last().DebugString() << std::endl;
//   std::cout << pen::resultString.find(history.ComputeGameResult())->second
//             << std::endl;
// }

}  // namespace pen