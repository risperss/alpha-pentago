#include <pybind11/operators.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <string>

#include "neural/heuristic.h"
#include "neural/negamax.h"
#include "pentago/position.h"

#define STRINGIFY(x) #x
#define MACRO_STRINGIFY(x) STRINGIFY(x)

namespace py = pybind11;

PYBIND11_MODULE(pentago, m) {
  m.doc() = "Pentago Game and Engine";

  // used for cmaes
  m.attr("NUM_WEIGHTS") = pentago::kGenomeLength;

  py::class_<pentago::Move>(m, "Move")
      .def(py::init<const std::string&>())
      .def("__str__", &pentago::Move::as_string)
      .def(py::self == py::self)
      .def(py::self != py::self);

  py::enum_<pentago::GameResult>(m, "GameResult")
      .value("WHITE_WON", pentago::GameResult::WHITE_WON)
      .value("BLACK_WON", pentago::GameResult::BLACK_WON)
      .value("DRAW", pentago::GameResult::DRAW)
      .value("UNDECIDED", pentago::GameResult::UNDECIDED)
      .export_values();

  py::class_<pentago::Position>(m, "Position")
      .def(py::init<const pentago::Position&, pentago::Move&>())
      .def(py::init<const std::string&>())
      .def("__str__", &pentago::Position::DebugString)
      .def("compute_result", &pentago::Position::ComputeGameResult)
      .def("legal_moves", &pentago::Position::GenerateLegalMoves)
      .def("white_pieces", &pentago::Position::white_pieces)
      .def("black_pieces", &pentago::Position::black_pieces);

  py::class_<pentago::PositionHistory>(m, "PositionHistory")
      .def(py::init<>())
      .def("push", pybind11::overload_cast<pentago::Move>(
                       &pentago::PositionHistory::Append))
      .def("pop", &pentago::PositionHistory::Pop)
      .def("compute_result", &pentago::PositionHistory::ComputeGameResult)
      .def("get_last", &pentago::PositionHistory::Last)
      .def("__len__", &pentago::PositionHistory::GetLength);

  py::class_<pentago::NReturn>(m, "NReturn")
      .def_readwrite("value", &pentago::NReturn::value)
      .def_readwrite("move", &pentago::NReturn::move);

  py::class_<pentago::Negamax>(m, "Negamax")
      .def(py::init<>())
      .def(py::init<pentago::Genome>())
      .def("best", &pentago::Negamax::best)
      .def("get_nodes_visited", &pentago::Negamax::getNodesVisited);

#ifdef VERSION_INFO
  m.attr("__version__") = MACRO_STRINGIFY(VERSION_INFO);
#else
  m.attr("__version__") = "dev";
#endif
}