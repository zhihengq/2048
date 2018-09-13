#ifndef _AI_GRADIENT_LINEAR_4X4_WEIGHT_EVALUATOR_H_
#define _AI_GRADIENT_LINEAR_4X4_WEIGHT_EVALUATOR_H_

#include <cstdint>

#include "ai/weight_table_evaluator.h"

namespace _2048 {
namespace ai {

/**
 * @copydoc
 * The weight table is 4 by 4.
 * The weight decreases linearly from the lower-left corner to the upper-right corner.
 */
class GradientLinear4x4WeightEvaluator : public WeightTableEvaluator {
 public:
    /**
     * Constructor
     */
    GradientLinear4x4WeightEvaluator()
            : WeightTableEvaluator(4, 4, kWeights_) { }

 private:
    static const int64_t kWeights_[];
};

}  // namespace ai
}  // namespace _2048

#endif  // _AI_GRADIENT_LINEAR_4X4_WEIGHT_EVALUATOR_H_
