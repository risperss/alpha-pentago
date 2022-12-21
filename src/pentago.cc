#include <neural/cmaes.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(pentagolib, m) {
  m.doc() = "Alpha Pentago Python Library";

  m.def("fitnesses", &pentago::fitnesses);
  m.attr("NUM_WEIGHTS") = pentago::kNumWeights;
}