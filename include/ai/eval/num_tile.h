#ifndef _AI_EVAL_NUMTILE_H_
#define _AI_EVAL_NUMTILE_H_

#include <cstdint>

#include "ai/eval/eval_func.h"

namespace _2048 {
namespace ai {
namespace eval {

/**
 * Evaluation functions based on number of tiles on grid.
 */
class NumTile : public EvaluationFunction {
 public:
    int64_t operator()(const GameState &state) const override;
};

}  // namespace eval
}  // namespace ai
}  // namespace _2048

#endif  // _AI_EVAL_NUMTILE_H_
