#include <array>

#include "neural/heuristic.h"
#include "pentago/position.h"

namespace pentago {
using Chromosome = std::array<float, kNumWeights>;

GameResult versus_game(Chromosome white, Chromosome black, int search_depth);
std::pair<int, int> match(Chromosome greece, Chromosome persia, int num_games,
                          int search_depth);
std::vector<float> fitnesses(std::vector<Chromosome> generation);
}  // namespace pentago