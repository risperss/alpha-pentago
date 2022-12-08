#include <cstdint>
#include <unordered_map>

namespace pen {
std::unordered_map<std::uint64_t, int>* smartClearedLookup(
    std::unordered_map<std::uint64_t, int>* lookup);

void selfPlay();
void vsHuman();
}  // namespace pen