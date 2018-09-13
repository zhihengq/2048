#include "ai/eval/weight_table/gradient_exponential_4x4.h"

#include <cstdint>

namespace _2048 {
namespace ai {
namespace eval {
namespace weight_table {

const int64_t GradientExponential4x4::kWeights_[] = {
    -8, -16, -32, -64,
    -4,  -8, -16, -32,
    -2,  -4,  -8, -16,
    -1,  -2,  -4,  -8 };

}  // namespace weight_table
}  // namespace eval
}  // namespace ai
}  // namespace _2048
