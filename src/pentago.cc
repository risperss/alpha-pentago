#include <neural/cmaes.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

PYBIND11_MODULE(pentago, handle) {
  handle.doc() = "Alpha Pentago Python Library.";

  handle.def("fitness", &pentago::fitness);
}