#include "position.h"

#include <cstdint>

namespace pen {

Position::Position(const PentagoBoard& parent, Move m)
	: move_count_(parent.move_count_ + 1) {

	board_.our_pieces_ = parent.their_pieces_;
	board_.their_pieces_ = parent.our_pieces_;
}

std::string Position::DebugString() const { return board_.DebugString(); }

GameResult PositionHistory::ComputeGameResult() const {
	GameResult result = Last().GetBoard().ComputeGameResult();

	if (result == GameResult::UNDECIDED || result == GameResult::DRAW) {
		return result;
	} else {
		return BlackToMove() ? 
	}
}

} // namespace pen