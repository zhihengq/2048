#include <cstdint>
#include <ncurses.h>
#include "game_state.h"
#include "ncurses_controller.h"

namespace _2048 {

// instance
std::unique_ptr<NcursesController> NcursesController::instance_;

// play
bool NcursesController::Play(const GameState &state,
                             GameState::Direction *move) {
    last_key_ = getch();
    switch (last_key_) {
        case KEY_LEFT:
            if (move != nullptr) *move = GameState::Direction::LEFT;
            return true;
        case KEY_RIGHT:
            if (move != nullptr) *move = GameState::Direction::RIGHT;
            return true;
        case KEY_UP:
            if (move != nullptr) *move = GameState::Direction::UP;
            return true;
        case KEY_DOWN:
            if (move != nullptr) *move = GameState::Direction::DOWN;
            return true;
        default:
            return false;
    }
}

}  // namespace _2048
