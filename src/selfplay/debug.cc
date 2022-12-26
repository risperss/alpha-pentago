#include "debug.h"

#include <chrono>
#include <iostream>
#include <random>
#include <string>

#include "neural/heuristic.h"
#include "neural/minimax.h"
#include "pentago/position.h"
#include "utils/bitops.h"
#include "utils/hashcat.h"
#include "utils/lookup.h"

namespace pentago {
void selfPlay(int depth) {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  PositionLookup* position_lookup = new PositionLookup;
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
    ReturnValue result = minimax(history.Last(), depth, position_lookup,
                                 &nodesVisited, evaluator);
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    unsigned long lookupSize = position_lookup->size() * sizeof(LookupItem);

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

    position_lookup = clearedLookup(position_lookup, history.Last());
    history.Append(result.move);
  }
  delete position_lookup;

  std::cout << "Ply Count:\t" << history.Last().GetPlyCount() << "\n";
  std::cout << "Result:\t\t"
            << resultString.find(history.ComputeGameResult())->second << "\n";
  std::cout << "Max RAM:\t" << (maxLookupSize >> 20) << " Mb\n";
  std::cout << "Search Depth:\t" << depth << std::endl;
  std::cout << "AVG NPS:\t" << (totalNodesVisited / totalTimeTaken)
            << " kN/s\n";
  std::cout << history.Last().DebugString() << "\n";
}
}  // namespace pentago