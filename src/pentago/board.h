#pragma once

#include <cstdint>
#include <map>
#include <string>

#include "pentago/bitboard.h"
#include "utils/bitops.h"

namespace pentago {

enum class BoardResult : uint8_t { UNDECIDED, THEY_WON, DRAW, WE_WON };

static const float kMaxBoardValue = 999999.0;
static const float kMinBoardValue = -999999.0;

const std::map<BoardResult, float> kBoardResultValue{
    {BoardResult::WE_WON, kMaxBoardValue},
    {BoardResult::THEY_WON, kMinBoardValue},
    {BoardResult::DRAW, 0}};

class PentagoBoard {
 public:
  PentagoBoard();

  // Gaetano Rispoli Notation!
  PentagoBoard(const std::string& grn);

  void Clear();

  void SetFromGrn(std::string grn);

  void ApplyMove(Move move);

  bool IsLegalMove(Move move) const;

  MoveList SmartGenerateLegalMoves() const;

  MoveList GenerateLegalMoves() const;

  BoardResult ComputeBoardResult() const;

  int piece_count() const {
    return our_pieces_.count() + their_pieces_.count();
  }

  bool full() const { return piece_count() == 36; }

  void SwapBitBoards() { swap(our_pieces_, their_pieces_); }

  std::string DebugString(bool blackToMove) const;

  BitBoard our_pieces() const { return our_pieces_; }

  BitBoard their_pieces() const { return their_pieces_; }

 private:
  BitBoard our_pieces_;
  BitBoard their_pieces_;
};

}  // namespace pentago
