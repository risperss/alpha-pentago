#include "game.h"

#include <iostream>

#include "neural/minimax.h"
#include "pentago/position.h"

namespace pen {
void smartClearLookup(std::unordered_map<std::uint64_t, int>* lookup) {
  std::unordered_map<std::uint64_t, int>* newLookup =
      new std::unordered_map<std::uint64_t, int>;

  for (auto& [hash, eval] : *lookup) {
    if (eval == MAX_POSITION_VALUE || eval == -MAX_POSITION_VALUE) {
      (*newLookup)[hash] = eval;
    }
  }

  delete lookup;
  lookup = newLookup;
}

void selfPlay() {
  std::unordered_map<std::uint64_t, int>* lookup =
      new std::unordered_map<std::uint64_t, int>;

  pen::PentagoBoard board = pen::PentagoBoard();
  pen::Position starting = pen::Position(board);
  pen::PositionHistory history = pen::PositionHistory(starting);

  while (history.ComputeGameResult() == pen::GameResult::UNDECIDED) {
    std::pair<pen::Move, int> value = pen::value(history.Last(), lookup);

    std::cout << "Ply Count: " << history.Last().GetPlyCount() << std::endl;
    std::cout << "To Move: "
              << (history.Last().IsBlackToMove() ? "Black" : "White")
              << std::endl;
    std::cout << "Value: " << std::get<int>(value) << std::endl;
    std::cout << "Move to be made: " << std::get<pen::Move>(value).as_string()
              << std::endl;
    std::cout << history.Last().DebugString() << std::endl;
    std::cout << "------------------------------" << std::endl;

    smartClearLookup(lookup);
    history.Append(std::get<pen::Move>(value));
  }
  delete lookup;

  std::cout << history.Last().DebugString() << std::endl;
  std::cout << pen::resultString.find(history.ComputeGameResult())->second
            << std::endl;
  std::cout << "Depth: " << pen::DEPTH << std::endl;
}

void vsHuman() {
  std::unordered_map<std::uint64_t, int>* lookup =
      new std::unordered_map<std::uint64_t, int>;

  pen::PentagoBoard board = pen::PentagoBoard();
  pen::Position starting = pen::Position(board);
  pen::PositionHistory history = pen::PositionHistory(starting);

  bool humanMove = true;

  while (history.ComputeGameResult() == pen::GameResult::UNDECIDED) {
    pen::Move move;

    if (humanMove) {
      std::cout << history.Last().DebugString() << std::endl;

      std::string moveString;

      std::cout << "Enter a move: ";
      std::cin >> moveString;

      move = pen::Move(moveString);
    } else {
      std::pair<pen::Move, int> value = pen::value(history.Last(), lookup);

      move = std::get<pen::Move>(value);

      std::cout << "CPU Played: " << move.as_string() << std::endl;
    }

    smartClearLookup(lookup);
    history.Append(move);
    humanMove = !humanMove;
  }

  std::cout << history.Last().DebugString() << std::endl;
  std::cout << pen::resultString.find(history.ComputeGameResult())->second
            << std::endl;
}

}  // namespace pen