#pragma once

#include <string>

#include "board.h"

namespace pen {

class Position {
public:
    Position(const PentagoBoard& board);

    std::uint64_t Hash() const;
    bool IsTheirToMove() const { return move_count_ % 2 == 1; }

    int GetMoveCount() const { return move_count_; }

    const PentagoBoard& GetBoard() const { return board_; }

    std::string DebugString() const;

private:
    PentagoBoard board_;

    int move_count_ = 0;
};

class PositionHistory {
public:
    PositionHistory() = default;

    const Position& Starting() const { return positions_.front(); }
    const Position& Last() const { return positions_.back(); }
    const Position& GetPositionAt(int idx) const { return positions_[idx]; }

    void Reserve(int size) { positions_.reserve(size); }
    int GetLength() const { return positions_.size(); }

    void Append(Move m);
    void Pop() { positions_.pop_back(); }

    GameResult ComputeGameResult() const;

    bool IsTheirToMove() const { return Last().IsTheirToMove(); }

private:
    std::vector<Position> positions_;
};

} // namespace pen