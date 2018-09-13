#include "ai/gradient_linear_4x4_weight_evaluator.h"

#include <cstdint>

namespace _2048 {
namespace ai {

const int64_t GradientLinear4x4WeightEvaluator::kWeights_[] = {
    -4, -5, -6, -7,
    -3, -4, -5, -6,
    -2, -3, -4, -5,
    -1, -2, -3, -4 };

}  // namespace ai
}  // namespace _2048
