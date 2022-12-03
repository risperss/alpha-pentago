#include <cstdint>

namespace pen {
    int count(const std::uint64_t board_) {
#if defined(NO_POPCNT)
        std::uint64_t x = board_;
            x -= (x >> 1) & 0x5555555555555555;
            x = (x & 0x3333333333333333) + ((x >> 2) & 0x3333333333333333);
            x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0F;
            return (x * 0x0101010101010101) >> 56;
#elif defined(_MSC_VER) && defined(_WIN64)
        return _mm_popcnt_u64(board_);
#elif defined(_MSC_VER)
        return __popcnt(board_) + __popcnt(board_ >> 32);
#else
        return __builtin_popcountll(board_);
#endif
    }
}