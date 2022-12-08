#include <unordered_map>

#include "pentago/bitboard.h"
#include "pentago/position.h"

namespace pen {
static const int MAX_POSITION_VALUE = 1000;
static const int DEPTH = 3;

struct ReturnValue {
  Move move;
  int value;
  bool leaf;
};

std::pair<Move, int> minimax(Position position, Move prevMove, int depth,
                             int alpha, int beta,
                             std::unordered_map<std::uint64_t, int>* lookup);

std::pair<Move, int> minimax(Position position,
                             std::unordered_map<std::uint64_t, int>* lookup);
}  // namespace pen