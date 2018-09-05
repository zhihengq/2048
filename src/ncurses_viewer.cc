#include <cstdint>
#include <tuple>
#include <sstream>
#include <iomanip>
#include <csignal>
#include <ncurses.h>
#include "game_state.h"
#include "ncurses_viewer.h"

namespace _2048 {

typedef std::tuple<uint32_t, uint32_t> Coord2D;
inline uint8_t GetMaxWidth(const GameState &state) noexcept;
inline Coord2D GetDimension(uint32_t height, uint32_t width,
                            uint8_t max_width) noexcept;
inline void Draw(const GameState &state,
                 const Coord2D &v, uint8_t max_width) noexcept;

// instance
NcursesViewer NcursesViewer::viewer;

// resize handler
void ResizeHandler(int sig) noexcept {
    NcursesViewer::viewer.saved_handler_(sig);
    NcursesViewer::viewer.Refresh();
}

// constructor
NcursesViewer::NcursesViewer() noexcept {
    initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    attron(A_BOLD);
    saved_handler_ = signal(SIGWINCH, ResizeHandler);
}

// destructor
NcursesViewer::~NcursesViewer() noexcept {
    signal(SIGWINCH, saved_handler_);
    endwin();
}

// update
void NcursesViewer::Update(const GameState &state) {
    saved_state_.reset(new GameState(state));
    Refresh();
}

// refresh
void NcursesViewer::Refresh() noexcept {
    unsigned int scr_height, scr_width;
    getmaxyx(stdscr, scr_height, scr_width);
    uint8_t max_width = GetMaxWidth(*saved_state_);
    Coord2D dim = GetDimension(saved_state_->height(),
                               saved_state_->width(), max_width);
    uint32_t r = scr_height < std::get<0>(dim) ?
            0 : (scr_height - std::get<0>(dim)) / 2;
    uint32_t c = scr_width < std::get<1>(dim) ?
            0 : (scr_width - std::get<1>(dim)) / 2;
    Draw(*saved_state_, std::make_tuple(r, c), max_width);
    move(0, 0);
}

inline uint8_t GetMaxWidth(const GameState &state) noexcept {
    uint8_t max = 0;
    for (uint32_t r = 0; r < state.height(); r++)
        for (uint32_t c = 0; c < state.width(); c++) {
            // width = log_10(2^power) = power * log_10(2)
            uint8_t power = state.tile(GameState::Position(r, c)).power();
            constexpr float kLog_10_2 = 0.30102999566;
            uint8_t w = static_cast<uint8_t>(kLog_10_2 * power) + 1;
            if (max < w)
                max = w;
        }
    return max;
}

inline Coord2D GetDimension(uint32_t height, uint32_t width,
                            uint8_t max_width) noexcept {
    return std::make_tuple(height * 2 + 1, width * (max_width + 1) + 1);
}

inline void Draw(const GameState &state,
                 const Coord2D &v, uint8_t max_width) noexcept {
    erase();
    for (uint32_t r = 0; r < state.height(); r++) {
        // draw upper boarder
        if (r == 0) {
            move(std::get<0>(v), std::get<1>(v));
            addch(ACS_ULCORNER);
            for (uint32_t c = 0; c < state.width(); c++) {
                for (uint8_t i = 0; i < max_width; i++)
                    addch(ACS_HLINE);
                addch(c + 1 == state.width() ? ACS_URCORNER : ACS_TTEE);
            }
        }

        // draw content
        move(std::get<0>(v) + r * 2 + 1, std::get<1>(v));
        addch(ACS_VLINE);
        for (uint32_t c = 0; c < state.width(); c++) {
            if (state.tile(GameState::Position(r, c)).empty()) {
                for (uint8_t i = 0; i < max_width; i++)
                    addch(' ');
            } else {
                std::ostringstream oss;
                oss << std::setw(max_width)
                        << state.tile(GameState::Position(r, c));
                addstr(oss.str().c_str());
            }
            addch(ACS_VLINE);
        }

        // draw lower line
        move(std::get<0>(v) + r * 2 + 2, std::get<1>(v));
        if (r + 1 == state.height()) {
            addch(ACS_LLCORNER);
            for (uint32_t c = 0; c < state.width(); c++) {
                for (uint8_t i = 0; i < max_width; i++)
                    addch(ACS_HLINE);
                addch(c + 1 == state.width() ? ACS_BTEE : ACS_LRCORNER);
            }
        } else {
            addch(ACS_LTEE);
            for (uint32_t c = 0; c < state.width(); c++) {
                for (uint8_t i = 0; i < max_width; i++)
                    addch(ACS_HLINE);
                addch(c + 1 == state.width() ? ACS_PLUS : ACS_RTEE);
            }
        }
    }
    refresh();
}

}  // namespace _2048
