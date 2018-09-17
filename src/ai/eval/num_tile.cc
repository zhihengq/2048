#include "ai/eval/num_tile.h"

#include <cstdint>
#include <stdexcept>

namespace _2048 {
namespace ai {
namespace eval {

// operator()
int64_t NumTile::operator()(const GameState &state) {
    int64_t sum = 0;
    for (uint32_t r = 0; r < state.height(); r++)
        for (uint32_t c = 0; c < state.width(); c++) {
            const Tile &tile = state.tile(GameState::Position(r, c));
            sum += tile.empty() ? 0 : -1;
        }
    return sum;
}

}  // namespace eval
}  // namespace ai
}  // namespace _2048
