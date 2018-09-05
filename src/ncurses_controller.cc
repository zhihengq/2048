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
    do {
        switch (getch()) {
            case KEY_LEFT:
                if (move != nullptr) *move = GameState::Direction::LEFT;
                break;
            case KEY_RIGHT:
                if (move != nullptr) *move = GameState::Direction::RIGHT;
                break;
            case KEY_UP:
                if (move != nullptr) *move = GameState::Direction::UP;
                break;
            case KEY_DOWN:
                if (move != nullptr) *move = GameState::Direction::DOWN;
                break;
            default:
                continue;
        }
    } while (false);
    return true;
}

}  // namespace _2048
