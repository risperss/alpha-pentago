#pragma once

#include <map>
#include <string>

#include "pentago/board.h"

namespace pentago {

enum class GameResult : uint8_t { UNDECIDED, BLACK_WON, DRAW, WHITE_WON };

const std::map<GameResult, std::string> resultString{
    {GameResult::WHITE_WON, "White Win"},
    {GameResult::BLACK_WON, "Black Win"},
    {GameResult::DRAW, "Draw"},
    {GameResult::UNDECIDED, "Undecided"}};

const std::map<GameResult, float> kTerminalResultValue{
    {GameResult::WHITE_WON, kMaxBoardValue},
    {GameResult::BLACK_WON, kMinBoardValue},
    {GameResult::DRAW, 0}};

GameResult operator-(const GameResult& res);

class Position {
 public:
  Position(const Position& parent, Move m);

  Position(const PentagoBoard& board);

  Position(const std::string& grn);

  bool IsBlackToMove() const { return ply_count_ % 2 == 1; }

  int GetPlyCount() const { return ply_count_; }

  MoveList GenerateLegalMoves();

  const PentagoBoard& GetBoard() const { return board_; }

  std::string DebugString() const;

  GameResult ComputeGameResult() const;

 private:
  PentagoBoard board_;

  int ply_count_ = 0;
};

using PositionList = std::vector<Position>;

class PositionHistory {
 public:
  PositionHistory();

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
  PositionList positions_;
};

}  // namespace pentago
