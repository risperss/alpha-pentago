#include "position.h"

#include <cstdint>

namespace pen {

Position::Position(const Position& parent, Move m)
	: move_count_(parent.move_count_ + 1) {
	board_.ApplyMove(m);
}

std::string Position::DebugString() const { 
	return board_.DebugString(IsBlackToMove()); 
}

GameResult operator-(const GameResult& res) {
  return res == GameResult::BLACK_WON   ? GameResult::WHITE_WON
         : res == GameResult::WHITE_WON ? GameResult::BLACK_WON
                                        : res;
}

GameResult PositionHistory::ComputeGameResult() const {
	BoardResult result = Last().GetBoard().ComputeBoardResult();

	if (result == BoardResult::UNDECIDED) {
		return GameResult::UNDECIDED;
	} else if (result == BoardResult::DRAW) {
		return GameResult::DRAW;
	} else if (result == BoardResult::WE_WON) {
		return IsBlackToMove() ? GameResult::WHITE_WON : GameResult::BLACK_WON;
	} else {
		return IsBlackToMove() ? GameResult::BLACK_WON : GameResult::WHITE_WON;
	}
}

void PositionHistory::Append(Move m) {
	positions_.emplace_back(Position(Last()), m);
}

} // namespace pen
