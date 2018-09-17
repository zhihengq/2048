#include "ai/eval/weight_table/zigzag_linear_4x4.h"

#include <cstdint>

namespace _2048 {
namespace ai {
namespace eval {
namespace weight_table {

const int64_t ZigzagLinear4x4::kWeights_[] = {
    -16, -15, -14, -13,
     -9, -10, -11, -12,
     -8,  -7,  -6,  -5,
     -1,  -2,  -3,  -4 };

}  // namespace weight_table
}  // namespace eval
}  // namespace ai
}  // namespace _2048
