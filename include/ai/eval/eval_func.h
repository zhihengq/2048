#ifndef _AI_EVAL_EVALFUNC_H_
#define _AI_EVAL_EVALFUNC_H_

#include <cstdint>
#include <limits>

#include "game_state.h"

namespace _2048 {
namespace ai {
namespace eval {

/**
 * Interface of a game state evaluation function for minimax.
 *
 * A evaluation function maps a game state to a number that describes the value
 * of the state to the player. The higher the number is, the greater value the
 * state is to the player, and the smaller value to the generator.
 */
class EvaluationFunction {
 public:
    /** Positive infinity */
    static constexpr int64_t kPosInf =  std::numeric_limits<int64_t>::max();
    /** Negative infinity */
    static constexpr int64_t kNegInf = std::numeric_limits<int64_t>::min();
    /** Game over */
    static constexpr int64_t kOver = -1000000000000000000;

    /**
     * Evaluate the game state.
     * @param state the game state to be evaluated
     * @return the value of the state (to the player)
     * @throw std::runtime_error if `state` is not valid
     */
    virtual int64_t operator()(const GameState &state) const = 0;

    /**
     * Destructor
     */
    virtual ~EvaluationFunction() noexcept { }
};

}  // namespace eval
}  // namespace ai
}  // namespace _2048

#endif  // _AI_EVAL_EVALFUNC_H_
