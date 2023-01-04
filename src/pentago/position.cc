#include "position.h"

#include <string>

#include "pentago/board.h"

namespace pentago {

Position::Position(const Position& parent, Move m)
    : ply_count_(parent.ply_count_ + 1) {
  board_ = parent.GetBoard();
  board_.ApplyMove(m);
  board_.SwapBitBoards();
}

Position::Position(const PentagoBoard& board) {
  board_ = board;
  ply_count_ = board.our_pieces().count() + board.their_pieces().count();
}

Position::Position(const std::string& grn) { Position(PentagoBoard(grn)); }

std::string Position::DebugString() const {
  return board_.DebugString(IsBlackToMove());
}

MoveList Position::GenerateLegalMoves() {
  return GetBoard().GenerateLegalMoves();
}

GameResult operator-(const GameResult& res) {
  return res == GameResult::BLACK_WON   ? GameResult::WHITE_WON
         : res == GameResult::WHITE_WON ? GameResult::BLACK_WON
                                        : res;
}

GameResult Position::ComputeGameResult() const {
  BoardResult board_result = GetBoard().ComputeBoardResult();
  GameResult game_result;

  if (board_result == BoardResult::UNDECIDED) {
    game_result = GameResult::UNDECIDED;
  } else if (board_result == BoardResult::DRAW) {
    game_result = GameResult::DRAW;
  } else if (board_result == BoardResult::WE_WON) {
    game_result = GameResult::WHITE_WON;
  } else {
    game_result = GameResult::BLACK_WON;
  }

  return IsBlackToMove() ? -game_result : game_result;
}

const std::uint64_t Position::white_pieces() const {
  return IsBlackToMove() ? GetBoard().their_pieces().as_int()
                         : GetBoard().our_pieces().as_int();
}

const std::uint64_t Position::black_pieces() const {
  return IsBlackToMove() ? GetBoard().our_pieces().as_int()
                         : GetBoard().their_pieces().as_int();
}

PositionHistory::PositionHistory() {
  positions_.emplace_back(Position(PentagoBoard()));
}

PositionHistory::PositionHistory(Position& starting) {
  positions_.emplace_back(starting);
}

void PositionHistory::Append(Move m) {
  if (positions_.empty()) {
    positions_.emplace_back(Position(PentagoBoard()));
  }
  positions_.emplace_back(Position(Last(), m));
}

}  // namespace pentago
