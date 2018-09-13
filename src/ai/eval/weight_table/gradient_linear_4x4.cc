#include "ai/eval/weight_table/gradient_linear_4x4.h"

#include <cstdint>

namespace _2048 {
namespace ai {
namespace eval {
namespace weight_table {

const int64_t GradientLinear4x4::kWeights_[] = {
    -4, -5, -6, -7,
    -3, -4, -5, -6,
    -2, -3, -4, -5,
    -1, -2, -3, -4 };

}  // namespace weight_table
}  // namespace eval
}  // namespace ai
}  // namespace _2048
