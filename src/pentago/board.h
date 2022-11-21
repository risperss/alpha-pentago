#pragma once

#include <cstdint>

#include "bitboard.h"

namespace pen {

class PentagoBoard {
public:
    PentagoBoard() = default;

    void ApplyMove(Move move);

    bool IsLegalMove(Move move) const;
    MoveList GenerateLegalMoves() const;

    std::string DebugString() const;

    BitBoard our_pieces() const { return our_pieces_; }
    BitBoard their_pieces() const { return their_pieces_; }

private:
    BitBoard our_pieces_;
    BitBoard their_pieces_;
};

} // namespace pen