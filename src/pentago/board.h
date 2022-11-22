#pragma once

#include <cstdint>

#include "bitboard.h"

namespace pen {

enum class GameResult : uint8_t { UNDECIDED, THEIR_WON, DRAW, OUR_WON };
GameResult operator-(const GameResult& res);

class PentagoBoard {
public:
    PentagoBoard() = default;

    void ApplyMove(Move move);

    bool IsLegalMove(Move move) const;
    MoveList GenerateLegalMoves() const;

    GameResult ComputeGameResult() const;

    bool full() const {
        return (our_pieces() & their_pieces()) == 0xFFFFFFFFF;
    }

    std::string DebugString() const;

    BitBoard our_pieces() const { return our_pieces_; }
    BitBoard their_pieces() const { return their_pieces_; }

private:
    BitBoard our_pieces_;
    BitBoard their_pieces_;
};

} // namespace pen
