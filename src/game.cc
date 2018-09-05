#include <exception>
#include "game_state.h"
#include "game.h"

namespace _2048 {

// reset
void Game::Reset(const GameState &state) {
    state_.reset(new GameState(state));
    if (viewer_ != nullptr)
        viewer_->Update(*state_);
}

// reset move
void Game::Reset(GameState &&state) {
    state_.reset(new GameState(std::move(state)));
    if (viewer_ != nullptr)
        viewer_->Update(*state_);
}

bool Game::NoMoreMove() const {
    if (state_ == nullptr)
        throw std::runtime_error("invalid game state");
    return state_->GetPossibleMoves().empty();
}

// generate
bool Game::Generate() {
    if (state_ == nullptr)
        throw std::runtime_error("invalid game state");
    if (generator_ == nullptr)
        return false;

    GameState::Position pos;
    uint8_t power;
    bool success = generator_->Generate(*state_, &pos, &power);
    if (success) {
        try {
            success = state_->GenerateTile(pos, power);
        } catch (const std::out_of_range &) {
            return false;
        }
    }
    if (success && viewer_ != nullptr)
        viewer_->Update(*state_);
    return success;
}

// play
bool Game::Play() {
    if (state_ == nullptr)
        throw std::runtime_error("invalid game state");
    if (player_ == nullptr)
        return false;

    GameState::Direction move;
    bool success = player_->Play(*state_, &move);
    if (success)
        success = state_->Move(move);
    if (success && viewer_ != nullptr)
        viewer_->Update(*state_);
    return success;
}

}  // namespace _2048
