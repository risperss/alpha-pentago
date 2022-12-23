#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "neural/cmaes.h"
#include "selfplay/game.h"

namespace py = pybind11;

PYBIND11_MODULE(alpha_pentago, m) {
  m.doc() = "Alpha Pentago Python Library";

  m.attr("NUM_WEIGHTS") = pentago::kNumWeights;

  m.def("fitnesses", &pentago::fitnesses);
  m.def("self_play", &pentago::selfPlay);
}