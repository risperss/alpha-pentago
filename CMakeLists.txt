cmake_minimum_required(VERSION 3.22)
project(alpha_pentago)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_BINARY_DIR ${CMAKE_SOURCE_DIR}/bin)
set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR})
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR})

include_directories(src)

add_executable(alpha_pentago
    src/neural/heuristic.h
    src/neural/heuristic.cc
    src/neural/minimax.h
    src/neural/minimax.cc
    src/pentago/bitboard.h
    src/pentago/board.h
    src/pentago/board.cc
    src/pentago/position.h
    src/pentago/position.cc
    src/selfplay/game.h
    src/selfplay/game.cc
    src/utils/bitops.h
    src/utils/exception.h
    src/utils/hashcat.h
    src/main.cc
)
