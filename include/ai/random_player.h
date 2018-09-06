#ifndef _AI_RANDOM_PLAYER_H_
#define _AI_RANDOM_PLAYER_H_

#include <cstdint>
#include <random>
#include <chrono>
#include "game_state.h"
#include "player.h"

namespace _2048 {
namespace ai {

class RandomPlayer : public Player {
 public:
    RandomGenerator(const RandomGenerator &) = delete;
    RandomGenerator &operator=(const RandomGenerator &) = delete;

    /**
     * Construct a RandomGenerator and use system time as seed.
     */
    RandomGenerator() : engine_(
            std::chrono::system_clock::now().time_since_epoch().count()) { }

    /**
     * Construct a RandomGenerator with the specified seed.
     */
    explicit RandomGenerator(uint64_t seed) : engine_(seed) { }

    bool Generate(const GameState &state, GameState::Position *pos,
                  uint8_t *power) override;

 private:
    std::default_random_engine engine_;     /**< Random engine */
};

}  // namespace ai
}  // namespace _2048

#endif  // _AI_RANDOM_PLAYER_H_
