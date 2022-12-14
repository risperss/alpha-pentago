#include "game.h"

#include <chrono>
#include <iostream>
#include <string>

#include "neural/heuristic.h"
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
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  PositionLookup* lookup = new PositionLookup;

  pen::PentagoBoard board = pen::PentagoBoard();
  pen::Position starting = pen::Position(board);
  pen::PositionHistory history = pen::PositionHistory(starting);

  unsigned long maxLookupSize = 0;
  long totalNodesVisited = 0;
  long totalTimeTaken = 0;

  while (history.ComputeGameResult() == pen::GameResult::UNDECIDED) {
    int nodesVisited = 0;

    auto t1 = high_resolution_clock::now();
    ReturnValue result = pen::minimax(history.Last(), lookup, &nodesVisited);
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    unsigned long lookupSize = lookup->size() * sizeof(LookupItem);

    std::cout << "Ply Count:\t" << history.Last().GetPlyCount() << "\n";
    std::cout << "To Move:\t"
              << (history.Last().IsBlackToMove() ? "Black" : "White") << "\n";
    std::cout << "Value:\t\t" << result.value << "\n";
    std::cout << "Heuristic:\t" << heuristic_value(history.Last()) << std::endl;
    std::cout << "Move:\t\t" << result.move.as_string() << "\n";
    std::cout << "Nodes visited:\t" << (nodesVisited / 1000) << "k\n";
    std::cout << "Search time:\t" << ms_int.count() << " ms\n";
    std::cout << "Lookup size:\t" << (lookupSize >> 20) << " Mb\n";
    if (ms_int.count() != 0) {
      std::cout << "NPS:\t\t" << (nodesVisited / ms_int.count()) << " kN/s\n";
    }
    std::cout << history.Last().DebugString() << "\n";

    maxLookupSize = std::max(maxLookupSize, lookupSize);

    if (result.value == MAX_POSITION_VALUE ||
        result.value == MIN_POSITION_VALUE) {
      std::string color =
          result.value == MAX_POSITION_VALUE ? "White" : "Black";
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
            << pen::resultString.find(history.ComputeGameResult())->second
            << "\n";
  std::cout << "Max RAM:\t" << (maxLookupSize >> 20) << "Mb\n";
  std::cout << "Search Depth:\t" << pen::DEPTH << std::endl;
  std::cout << "AVG NPS:\t" << (totalNodesVisited / totalTimeTaken)
            << " kN/s\n";
  std::cout << history.Last().DebugString() << "\n";
}
}  // namespace pen