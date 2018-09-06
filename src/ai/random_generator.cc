#include <cstdint>
#include "game_state.h"
#include "ai/random_generator.h"

namespace _2048 {
namespace ai {

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
        *power = std::bernoulli_distribution(0.1)(engine_) ? 2 : 1;
    }
    return true;
}

}  // namespace ai
}  // namespace _2048
