#include <cstdint>
#include <unordered_map>

#include "neural/minimax.h"

namespace pen {
std::unordered_map<std::uint64_t, ReturnValue>* smartClearedLookup(
    std::unordered_map<std::uint64_t, ReturnValue>* lookup);

void selfPlay();
void vsHuman();
}  // namespace pen