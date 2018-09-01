#ifndef _PLAYER_PLAYER_H_
#define _PLAYER_PLAYER_H_

#include <cstdint>
#include "game_state.h"

namespace _2048 {

/**
 * Interface of a player that apply moves.
 */
class Player {
 public:
    /**
     * Play a move.
     * If failed, `*move` will stay unmodified.
     * @param state the current game state
     * @param move output the direction of the next move
     * @return true if successful, otherwise false
     * @throw std::runtime_error if `state` is not valid
     */
    virtual bool Play(const GameState &state, GameState::Direction *move) = 0;

    /**
     * Destructor
     */
    virtual ~Player();
};

}  // namespace _2048

#endif  // _PLAYER_PLAYER_H_
