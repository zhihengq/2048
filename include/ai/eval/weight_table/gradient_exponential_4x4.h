#ifndef _AI_EVAL_WEIGHTTABLE_GRADIENTEXPONENTIAL4X4_H_
#define _AI_EVAL_WEIGHTTABLE_GRADIENTEXPONENTIAL4X4_H_

#include <cstdint>

#include "ai/eval/weight_table/weight_table.h"

namespace _2048 {
namespace ai {
namespace eval {
namespace weight_table {

/**
 * A weight table based evaluation function where the weight decreases
 * exponentially * from the lower-left corner to the upper-right corner.
 * The weight table is 4 by 4.
 */
class GradientExponential4x4 : public WeightTable {
 public:
    /**
     * Constructor
     */
    GradientExponential4x4() : WeightTable(4, 4, kWeights_) { }

 private:
    static const int64_t kWeights_[];
};

}  // namespace weight_table
}  // namespace eval
}  // namespace ai
}  // namespace _2048

#endif  // _AI_EVAL_WEIGHTTABLE_GRADIENTEXPONENTIAL4X4_H_
