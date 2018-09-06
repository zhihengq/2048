#include "ai/random_player.h"

namespace _2048 {
namespace ai {

// play
bool RandomPlayer::Play(const GameState &state, GameState::Direction *move) {
    if (move != nullptr) {
        auto choices = state.GetPossibleMoves();
        if (choices.empty())
            return false;
        auto select = std::uniform_int_distribution<
                decltype(choices)::size_type>(0, choices.size() - 1)(engine_);
        *move = choices[select];
    }
    return true;
}

}  // namespace ai
}  // namespace _2048
