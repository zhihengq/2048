#ifndef _GENERATOR_H_
#define _GENERATOR_H_

#include <cstdint>
#include "game_state.h"

namespace _2048 {

/**
 * Interface of a tile generator that creates new tiles in game.
 */
class Generator {
 public:
    /**
     * Generate a tile.
     * If the generation failed, `*pos` and `*power` will stay unmodified.
     * @param state the current game state
     * @param pos output the position of the new tile
     * @param power output the number in the new tile in terms of power of 2
     * @return true if a new tile is generated successfully, otherwise false
     * @throw std::runtime_error if `state` is not valid
     */
    virtual bool Generate(const GameState &state,
                          GameState::Position *pos, uint8_t *power) = 0;

    /**
     * Destructor
     */
    virtual ~Generator() { }
};

}  // namespace _2048

#endif  // _GENERATOR_H_
