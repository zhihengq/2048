#ifndef _AI_EVAL_WEIGHTTABLE_WEIGHTTABLE_H_
#define _AI_EVAL_WEIGHTTABLE_WEIGHTTABLE_H_

#include <cstdint>

#include "ai/eval/eval_func.h"

namespace _2048 {
namespace ai {
namespace eval {
namespace weight_table {

/**
 * Base class of evaluation functions based on a weight table.
 *
 * The value is calculated by adding tile values times their position weights.
 */
class WeightTable : public EvaluationFunction {
 public:
    WeightTable() = delete;
    WeightTable &operator=(const WeightTable &) = delete;

    /**
     * Copy Constructor
     * @param t the `WeightTable` to be copied
     */
    WeightTable(const WeightTable &t) noexcept
            : weights_(t.weights_), height_(t.height_), width_(t.width_) { }

    /**
     * @copydoc EvaluationFunction::operator()()
     * @throw std::invalid_argument if `state` and the weight table have
     *      different size
     */
    int64_t operator()(const GameState &state) const override;

 protected:
    /**
     * Constructor
     * The weight table must be at least `height` * `width`
     * @param height the height of weight table
     * @param width the width of weight table
     * @param weights pointer to the weight table
     * @throw std::invalid_argument if the size of the weight table is 0
     * @throw std::invalid_argument if `weights` is `nullptr`
     */
    WeightTable(uint32_t height, uint32_t width,
                const int64_t *weights);

 private:
    const int64_t *const weights_;
    const uint32_t height_;
    const uint32_t width_;

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
};

}  // namespace weight_table
}  // namespace eval
}  // namespace ai
}  // namespace _2048

#endif  // _AI_EVAL_WEIGHTTABLE_WEIGHTTABLE_H_
