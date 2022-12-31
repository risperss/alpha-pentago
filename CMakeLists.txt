cmake_minimum_required(VERSION 3.22)
project(pentago)

set(CMAKE_CXX_STANDARD 17)

find_package(pybind11)

include_directories(src)

pybind11_add_module(pentago
    src/neural/heuristic.h
    src/neural/heuristic.cc
    src/neural/negamax.h
    src/neural/negamax.cc
    src/pentago/bitboard.h
    src/pentago/board.h
    src/pentago/board.cc
    src/pentago/position.h
    src/pentago/position.cc
    src/utils/bitops.h
    src/utils/exception.h
    src/utils/hashcat.h
    src/main.cc
)
