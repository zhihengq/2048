#include "ai/eval/weight_table/zigzag_exponential_4x4.h"

#include <cstdint>

namespace _2048 {
namespace ai {
namespace eval {
namespace weight_table {

const int64_t ZigzagExponential4x4::kWeights_[] = {
    -32768, -16384,  -8192,  -4096,
      -256,   -512,  -1024,  -2048,
      -128,    -64,    -32,    -16,
        -1,     -2,     -4,     -8 };

}  // namespace weight_table
}  // namespace eval
}  // namespace ai
}  // namespace _2048
