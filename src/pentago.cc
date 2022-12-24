#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include "neural/cmaes.h"
#include "selfplay/game.h"

namespace py = pybind11;

PYBIND11_MODULE(alpha_pentago, m) {
  m.doc() = "Alpha Pentago Python Library";

  // used for cmaes
  m.attr("NUM_WEIGHTS") = pentago::kNumWeights;
  m.def("fitnesses", &pentago::fitnesses);

  py::class_<Move>(m, "Move");

  py::class_<Game>(m, "Game")
      .def(py::init<>())
      .def("set_max_search_depth", &Game::SetMaxSearchDepth)
      .def("legal_moves", &Game::LegalMoves)
      .def("push", &Game::MakeMove)
      .def("pop", &Game::UnmakeMove)
      .def("compute_result", &Game::ComputeResult)
      .def("best_move", &Game::BestMove)
}