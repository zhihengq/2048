#include <exception>
#include "game.h"

namespace _2048 {

// generate
bool Game::Generate() {
    if (generator_ == nullptr)
        return false;

    GameState::Position pos;
    uint8_t power;
    bool success = generator_->Generate(state_, &pos, &power);
    if (success) {
        try {
            state_.GenerateTile(pos, power);
        } catch (const std::logic_error &) {
            return false;
        }
    }
    return success;
}

// play
bool Game::Play() {
    if (player_ == nullptr)
        return false;

    GameState::Direction move;
    bool success = player_->Play(state_, &move);
    if (success)
        state_.Move(move);
    return success;
}

// destructor
Game::~Game() { }

}  // namespace _2048
