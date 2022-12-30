#include <array>
#include <cstdint>

#include "utils/bitops.h"

#pragma once
namespace pentago {

using HashList = std::array<uint64_t, 8>;

static uint64_t Hash(uint64_t val) {
  return 0xfad0d7f2fbb059f1ULL * (val + 0xbaad41cdcb839961ULL) +
         0x7acec0050bf82f43ULL * ((val >> 31) + 0xd571b3a92b1b2755ULL);
}

static uint64_t HashCat(uint64_t x, uint64_t y) {
  uint64_t hash = Hash(x);
  hash ^= 0x299799adf0d95defULL + Hash(y) + (hash << 6) + (hash >> 2);
  return hash;
}

static uint64_t Hash(Position position) {
  return HashCat(position.GetBoard().our_pieces().as_int(),
                 position.GetBoard().their_pieces().as_int());
}

static HashList PositionHashes(uint64_t x, uint64_t y) {
  return {HashCat(x, y),
          HashCat(rotate90(x), rotate90(y)),
          HashCat(rotate180(x), rotate180(y)),
          HashCat(rotate270(x), rotate270(y)),

          HashCat(mirror(x), mirror(y)),
          HashCat(mirror(rotate90(x)), mirror(rotate90(y))),
          HashCat(mirror(rotate180(x)), mirror(rotate180(y))),
          HashCat(mirror(rotate270(x)), mirror(rotate270(y)))};
}

static HashList PositionHashes(Position position) {
  return PositionHashes(position.GetBoard().our_pieces().as_int(),
                        position.GetBoard().their_pieces().as_int());
}

}  // namespace pentago