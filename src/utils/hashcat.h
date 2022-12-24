#include <array>
#include <cstdint>

#include "utils/bitops.h"

#pragma once
namespace pentago {

uint64_t Hash(uint64_t val) {
  return 0xfad0d7f2fbb059f1ULL * (val + 0xbaad41cdcb839961ULL) +
         0x7acec0050bf82f43ULL * ((val >> 31) + 0xd571b3a92b1b2755ULL);
}

uint64_t HashCat(uint64_t x, uint64_t y) {
  uint64_t hash = Hash(x);
  hash ^= 0x299799adf0d95defULL + Hash(y) + (hash << 6) + (hash >> 2);
  return hash;
}

uint64_t HashPosition(uint64_t x, uint64_t y) { return HashCat(x, y); }

uint64_t ReverseHashPosition(uint64_t x, uint64_t y) {
  return HashPosition(reverse(x), reverse(y));
}

std::array<uint64_t, 2> PositionHashes(uint64_t x, uint64_t y) {
  return {HashPosition(x, y), ReverseHashPosition(x, y)};
}

}  // namespace pentago