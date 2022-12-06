#include "position.h"

namespace pen {

Position::Position(const Position& parent, Move m)
    : ply_count_(parent.ply_count_ + 1)
{
    board_ = parent.GetBoard();
    board_.ApplyMove(m);
    board_.SwapBitBoards();
}

Position::Position(const PentagoBoard& board)
{
    board_ = board;
    ply_count_ = board.our_pieces().count() + board.their_pieces().count();
}

std::string Position::DebugString() const
{
    return board_.DebugString(IsBlackToMove());
}

GameResult Position::ComputeGameResult() const
{
    BoardResult result = GetBoard().ComputeBoardResult();

    if (result == BoardResult::UNDECIDED) {
        return GameResult::UNDECIDED;
    } else if (result == BoardResult::DRAW) {
        return GameResult::DRAW;
    } else if (result == BoardResult::WE_WON) {
        return IsBlackToMove() ? GameResult::BLACK_WON : GameResult::WHITE_WON;
    } else {
        return IsBlackToMove() ? GameResult::WHITE_WON : GameResult::BLACK_WON;
    }
}

GameResult operator-(const GameResult& res)
{
    return res == GameResult::BLACK_WON ? GameResult::WHITE_WON
        : res == GameResult::WHITE_WON  ? GameResult::BLACK_WON
                                        : res;
}

PositionHistory::PositionHistory(Position& starting)
{
    positions_.emplace_back(starting);
}

void PositionHistory::Append(Move m)
{
    if (positions_.empty()) {
        positions_.emplace_back(Position(PentagoBoard()));
    }
    positions_.emplace_back(Position(Last(), m));
}

} // namespace pen
