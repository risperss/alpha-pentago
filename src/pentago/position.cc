#include "position.h"

#include <cstdint>

namespace pen {

std::string Position::DebugString() const { return board_.DebugString(); }

GameResult PositionHistory::ComputeGameResult() const {
	return Last().GetBoard().ComputeGameResult();
}

} // namespace pen