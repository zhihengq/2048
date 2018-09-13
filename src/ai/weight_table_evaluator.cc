#include "ai/weight_table_evaluator.h"

#include <cstdint>
#include <stdexcept>

namespace _2048 {
namespace ai {

// operator()
int64_t WeightTableEvaluator::operator()(const GameState &state) {
    if (state.height() != height_ || state.width() != width_)
        throw std::invalid_argument(
                "GameState and wight table have mismatch size");
    int64_t sum = 0;
    for (uint32_t r = 0; r < height_; r++)
        for (uint32_t c = 0; c < width_; c++) {
            const Tile &tile = state.tile(GameState::Position(r, c));
            if (!tile.empty())
                sum += (1 << tile.power()) * weight(r, c);
        }
    return sum;
}

// constructor
WeightTableEvaluator::WeightTableEvaluator(uint32_t height, uint32_t width,
                                           const int64_t *weights)
        : weights_(weights), height_(height), width_(width) {
    if (height_ == 0 || width_ == 0)
        throw std::invalid_argument("The weight table size is 0");
    if (weights_ == nullptr)
        throw std::invalid_argument("The weight table is null");
}

}  // namespace ai
}  // namespace _2048
