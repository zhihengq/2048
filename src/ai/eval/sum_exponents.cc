#include "ai/eval/sum_exponents.h"

#include <cstdint>

namespace _2048 {
namespace ai {
namespace eval {

// operator()
int64_t SumExponents::operator()(const GameState &state) {
    int64_t sum = 0;
    for (uint32_t r = 0; r < state.height(); r++)
        for (uint32_t c = 0; c < state.width(); c++)
            sum -= state.tile(GameState::Position(r, c)).power();
    return sum;
}

}  // namespace eval
}  // namespace ai
}  // namespace _2048
