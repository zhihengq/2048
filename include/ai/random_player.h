#ifndef _AI_RANDOMPLAYER_H_
#define _AI_RANDOMPLAYER_H_

#include <cstdint>
#include <random>
#include <chrono>

#include "player.h"

namespace _2048 {
namespace ai {

/**
 * A `Player` that generates random moves.
 */
class RandomPlayer : public Player {
 public:
    RandomPlayer(const RandomPlayer &) = delete;
    RandomPlayer &operator=(const RandomPlayer &) = delete;

    /**
     * Construct a RandomPlayer and use system time as seed.
     */
    RandomPlayer() : engine_(
            std::chrono::system_clock::now().time_since_epoch().count()) { }

    /**
     * Construct a RandomPlayer with the specified seed.
     */
    explicit RandomPlayer(uint64_t seed) : engine_(seed) { }

    bool Play(const GameState &state, GameState::Direction *move) override;

 private:
    std::default_random_engine engine_;     /**< Random engine */
};

}  // namespace ai
}  // namespace _2048

#endif  // _AI_RANDOMPLAYER_H_
