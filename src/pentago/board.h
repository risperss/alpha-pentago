

#include "bitboard.h"

namespace pen {

class PentagoBoard {
public:
	PentagoBoard() = default;

	void ApplyMove(Move move);

	MoveList GenerateLegalMoves() const;

	std::string DebugString() const;

	BitBoard red_pieces() const { return red_pieces_; }
	BitBoard black_pieces() const { return black_pieces_; }

private:
	BitBoard red_pieces_;
	BitBoard black_pieces_;
};

} // namespace pen