#ifndef _AI_EVAL_SUMEXPONENTS_H_
#define _AI_EVAL_SUMEXPONENTS_H_

#include <cstdint>

#include "ai/eval/eval_func.h"

namespace _2048 {
namespace ai {
namespace eval {

/**
 * Evaluation functions based on sum of exponents of tiles.
 */
class SumExponents : public EvaluationFunction {
 public:
    int64_t operator()(const GameState &state) override;
};

}  // namespace eval
}  // namespace ai
}  // namespace _2048

#endif  // _AI_EVAL_SUMEXPONENTS_H_
