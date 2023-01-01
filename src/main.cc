#include <chrono>
#include <iostream>

#include "neural/heuristic.h"
#include "neural/negamax.h"

using namespace pentago;

int main(void) {
  using std::chrono::duration;
  using std::chrono::duration_cast;
  using std::chrono::high_resolution_clock;
  using std::chrono::milliseconds;

  for (int depth = 1; depth <= 5; depth++) {
    Negamax negamax = Negamax();

    PentagoBoard starting_board = PentagoBoard();
    Position starting_position = Position(starting_board);

    float value;
    auto t1 = high_resolution_clock::now();
    {  // Just for ease of reading
      value = negamax.value(starting_position, depth, 1);
    }
    auto t2 = high_resolution_clock::now();
    auto ms_int = duration_cast<milliseconds>(t2 - t1);

    std::cout << "Depth:\t" << depth << "\n";
    std::cout << "Value:\t" << value << "\n";

    if (negamax.getNodesVisited() > 10000000) {
      std::cout << "Nodes Visited:\t" << negamax.getNodesVisited() / 1000000
                << "m \n";
    } else if (negamax.getNodesVisited() > 10000) {
      std::cout << "Nodes Visited:\t" << negamax.getNodesVisited() / 1000
                << "k \n";
    } else {
      std::cout << "Nodes Visited:\t" << negamax.getNodesVisited() << "\n";
    }
    std::cout << "Time Elapsed:\t" << ms_int.count() << " ms\n";
    std::cout << "--------------------" << std::endl;
  }

  return 0;
}
