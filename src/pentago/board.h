#pragma once

#include <cstdint>

#include "bitboard.h"

namespace pen {

enum class GameResult : uint8_t { UNDECIDED, BLACK_WON, DRAW, RED_WON };
GameResult operator-(const GameResult& res);

class PentagoBoard {
public:
    PentagoBoard() = default;

    void ApplyMove(Move move);

    bool IsLegalMove(Move move) const;
    MoveList GenerateLegalMoves() const;

    GameResult ComputeGameResult() const;

    bool full() const {
        return red_pieces() & black_pieces() == 0xFFFFFFFFF;
    }

    std::string DebugString() const;

    BitBoard red_pieces() const { return red_pieces_; }
    BitBoard black_pieces() const { return black_pieces_; }

private:
    BitBoard red_pieces_;
    BitBoard black_pieces_;
};

} // namespace pen
