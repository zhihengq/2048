#ifndef _AI_WEIGHT_TABLE_EVALUATOR_H_
#define _AI_WEIGHT_TABLE_EVALUATOR_H_

#include <cstdint>
#include "ai/evaluation_function.h"

namespace _2048 {
namespace ai {

/**
 * A evaluation function based on a weight table.
 *
 * The value is calculated by adding tile values times their position weights.
 */
class WeightTableEvaluator : EvaluationFunction {
 public:
    WeightTableEvaluator() = delete;
    WeightTableEvaluator &operator=(const WeightTableEvaluator &) = delete;

    /**
     * Copy Constructor
     * @param t the `WeightTableEvaluator` to be copied
     */
    WeightTableEvaluator(const WeightTableEvaluator &t) noexcept
            : weights_(t.weights_), height_(t.height_), width_(t.width_) { }

    /**
     * @copydoc
     * @throw std::invalid_argument if `state` and the weight table have
     *      different size
     */
    int64_t operator()(const GameState &state) override;

 protected:
    const int64_t *const weights_;

    /**
     * Constructor
     * The weight table must be at least `height` * `width`
     * @param height the height of weight table
     * @param width the width of weight table
     * @param weights pointer to the weight table
     * @throw std::invalid_argument if the size of the weight table is 0
     * @throw std::invalid_argument if `weights` is `nullptr`
     */
    WeightTableEvaluator(uint32_t height, uint32_t width, int64_t *weights);

    /**
     * Retrive the weight.
     * Requires that `r` and `c` are in range
     * @param r the row index
     * @param c the col index
     * @return the weight
     */
    int64_t weight(uint32_t r, uint32_t c) noexcept {
        return weights_[r * width_ + c];
    }

 private:
    const uint32_t height_;
    const uint32_t width_;
};

}  // namespace ai
}  // namespace _2048

#endif  // _AI_WEIGHT_TABLE_EVALUATOR_H_
