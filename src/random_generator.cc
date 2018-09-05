#include <cstdint>
#include "game_state.h"
#include "random_generator.h"

namespace _2048 {

// generate
bool RandomGenerator::Generate(const GameState &state, GameState::Position *pos,
                               uint8_t *power) {
    if (pos != nullptr) {
        auto choices = state.GetEmptyTiles();
        if (choices.empty())
            return false;
        auto select = std::uniform_int_distribution<
                decltype(choices)::size_type>(0, choices.size() - 1)(engine_);
        *pos = choices[select];
    }
    if (power != nullptr) {
        *power = std::uniform_int_distribution<uint8_t>(1, 2)(engine_);
    }
    return true;
}

}  // namespace _2048
