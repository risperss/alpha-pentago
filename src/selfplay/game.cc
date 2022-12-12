#include "game.h"

#include <iostream>
#include <string>

#include "neural/minimax.h"
#include "pentago/position.h"

namespace pen {
PositionLookup* clearedLookup(PositionLookup* lookup, Position position) {
  if (position.GetPlyCount() + DEPTH >= 9) {
    return smartClearedLookup(lookup);
  } else {
    delete lookup;
    return new PositionLookup;
  }
}

PositionLookup* smartClearedLookup(PositionLookup* lookup) {
  PositionLookup* newLookup = new PositionLookup;

  for (auto& [hash, lookupItem] : *lookup) {
    if (lookupItem.value == MAX_POSITION_VALUE ||
        lookupItem.value == MIN_POSITION_VALUE) {
      (*newLookup)[hash] = lookupItem;
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
    std::cout << "Value:\t\t\t\t\t" << static_cast<int>(result.value) << "\n";
    std::cout << "Move to be made: " << result.move.as_string() << "\n";
    std::cout << history.Last().DebugString() << "\n";
    std::cout << "Hash: " << history.Last().Hash() << "\n";
    std::cout << "Reverse Hash: " << history.Last().ReverseHash() << "\n";
    std::cout << "Lookup length: " << lookup->size() << " positions\n";
    std::cout << "Lookup size: "
              << ((lookup->size() * sizeof(LookupItem)) / 1000000) << "Mb\n";
    if (result.value == MAX_POSITION_VALUE ||
        result.value == MIN_POSITION_VALUE) {
      std::string color =
          result.value == MAX_POSITION_VALUE ? "White" : "Black";
      std::cout << color << " to win on ply "
                << static_cast<int>(result.plyCount) << "\n";
    }
    std::cout << "------------------------------" << std::endl;

    lookup = clearedLookup(lookup, history.Last());
    history.Append(result.move);
  }
  delete lookup;

  std::cout << "Ply Count: " << history.Last().GetPlyCount() << "\n";
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