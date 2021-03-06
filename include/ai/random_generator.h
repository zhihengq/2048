#ifndef _AI_RANDOMGENERATOR_H_
#define _AI_RANDOMGENERATOR_H_

#include <cstdint>
#include <random>
#include <chrono>

#include "generator.h"

namespace _2048 {
namespace ai {

/**
 * A `Generator` that generates random tiles.
 * The probability distribution is the same as the original 2048 game.
 */
class RandomGenerator : public Generator {
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

#endif  // _AI_RANDOMGENERATOR_H_
