cmake_minimum_required(VERSION 3.22)
project(alpha_pentago)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)

add_executable(alpha_pentago
        src/pentago/bitboard.h
        src/pentago/board.cc
        src/pentago/board.h
        src/utils/hashcat.h
        src/neural/minimax.cc
        src/pentago/position.cc
        src/pentago/position.h src/utils/exception.h src/utils/bitops.h)
