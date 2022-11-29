#pragma once

#include <string>

#include "board.h"

namespace pen {

enum class GameResult : uint8_t { UNDECIDED, BLACK_WON, DRAW, WHITE_WON };
GameResult operator-(const GameResult& res);

class Position {
public:
    Position(const Position& parent, Move m);
    Position(const PentagoBoard& board);

    std::uint64_t Hash() const {
        return GetBoard().Hash();
    }

    bool IsBlackToMove() const { return move_count_ % 2 == 0; }
    int GetMoveCount() const { return move_count_; }

    const PentagoBoard& GetBoard() const { return board_; }

    std::string DebugString() const;

    GameResult ComputeGameResult() const;

private:
    PentagoBoard board_;

    int move_count_;
};

class PositionHistory {
public:
    PositionHistory();

    const Position& Starting() const { return positions_.front(); }
    const Position& Last() const { return positions_.back(); }
    const Position& GetPositionAt(int idx) const { return positions_[idx]; }

    void Reserve(int size) { positions_.reserve(size); }
    int GetLength() const { return positions_.size(); }

    void Append(Move m);
    void Append(Position p);
    void Pop() { positions_.pop_back(); }

    GameResult ComputeGameResult() const { return Last().ComputeGameResult(); }

    bool IsBlackToMove() const { return Last().IsBlackToMove(); }

private:
    std::vector<Position> positions_;
};

} // namespace pen
