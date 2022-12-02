cmake_minimum_required(VERSION 3.22)
project(alpha_pentago)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(alpha_pentago
        src/bitboard.h
        src/board.cc
        src/board.h
        src/hashcat.h
        src/minimax.cc
        src/position.cc
        src/position.h)
