#include "ui/ncurses_controller.h"

#include <ncurses.h>
#include <cstdint>
#include <sstream>

namespace _2048 {
namespace ui {

// instance
std::unique_ptr<NcursesController> NcursesController::instance_;

// resize handler
void NcursesController::NcursesControllerResizeHandler(int sig) noexcept {
    NcursesController &instance = NcursesController::instance();
    instance.saved_handler_(sig);
    instance.Redraw();
}

namespace {
std::u32string BuildStatus(const GameState &state) {
    std::basic_ostringstream<char32_t> oss;
    // Direction
    const auto moves = state.GetPossibleMoves();
    if (moves.empty()) {
        oss << U"No possible move";
    } else {
        oss << U"Directions: ";
        for (const auto &dir : state.GetPossibleMoves()) {
            switch (dir) {
                case GameState::Direction::LEFT:
                    oss << static_cast<char32_t>(ACS_LARROW) << U' '; break;
                case GameState::Direction::RIGHT:
                    oss << static_cast<char32_t>(ACS_RARROW) << U' '; break;
                case GameState::Direction::UP:
                    oss << static_cast<char32_t>(ACS_UARROW) << U' '; break;
                case GameState::Direction::DOWN:
                    oss << static_cast<char32_t>(ACS_DARROW) << U' '; break;
            }
        }
    }
    return oss.str();
}
}  // namespace

// play
bool NcursesController::Play(const GameState &state,
                             GameState::Direction *move) {
    SetStatusLine(BuildStatus(state));
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

}  // namespace ui
}  // namespace _2048
