#ifndef _AI_EVALUATION_FUNCTION_H_
#define _AI_EVALUATION_FUNCTION_H_

#include <cstdint>
#include <limits>

#include "game_state.h"

namespace _2048 {
namespace ai {

/**
 * Interface of a game state evaluation function for minimax.
 *
 * A evaluation function maps a game state to a number that describes the value
 * of the state to the player. The higher the number is, the greater value the
 * state is to the player, and the smaller value to the generator.
 */
class EvaluationFunction {
 public:
    /** The maximum value representable */
    static const int64_t kMaxValue =  std::numeric_limits<int64_t>::max();
    /** The minimum value representable (negative maximum) */
    static const int64_t kMinValue = -std::numeric_limits<int64_t>::max();

    /**
     * Evaluate the game state.
     * @param state the game state to be evaluated
     * @return the value of the state (to the player)
     * @throw std::runtime_error if `state` is not valid
     */
    virtual int64_t operator()(const GameState &state) = 0;

    /**
     * Destructor
     */
    virtual ~EvaluationFunction() noexcept { }
};

}  // namespace ai
}  // namespace _2048

#endif  // _AI_EVALUATION_FUNCTION_H_
