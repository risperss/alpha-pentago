#include <iostream>

#include "neural/cmaes.h"
#include "selfplay/game.h"

int main(void) {
  // pentago::selfPlay();
  pentago::fitness(pentago::kDefaultWeights);

  return 0;
}
