#pragma once

#include <map>
#include <string>

#include "pentago/board.h"

namespace pen {

enum class GameResult : uint8_t { UNDECIDED, BLACK_WON, DRAW, WHITE_WON };

const std::map<GameResult, std::string> resultString{
    {GameResult::WHITE_WON, "White Win"},
    {GameResult::BLACK_WON, "Black Win"},
    {GameResult::DRAW, "Draw"},
    {GameResult::UNDECIDED, "Undecided"}};

GameResult operator-(const GameResult& res);

class Position {
 public:
  Position(const Position& parent, Move m);

  Position(const PentagoBoard& board);

  std::uint64_t Hash() const { return GetBoard().Hash(); }

  std::uint64_t ReverseHash() const { return GetBoard().ReverseHash(); }

  bool IsBlackToMove() const { return ply_count_ % 2 == 1; }

  int GetPlyCount() const { return ply_count_; }

  const PentagoBoard& GetBoard() const { return board_; }

  std::string DebugString() const;

  GameResult ComputeGameResult() const;

 private:
  PentagoBoard board_;

  int ply_count_ = 0;
};

class PositionHistory {
 public:
  PositionHistory() = default;

  PositionHistory(Position& starting);

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

}  // namespace pen
