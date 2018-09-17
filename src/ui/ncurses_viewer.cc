#include "ui/ncurses_viewer.h"

#include <ncurses.h>
#include <cstdint>
#include <csignal>
#include <tuple>
#include <sstream>
#include <iomanip>

namespace _2048 {
namespace ui {

typedef std::tuple<uint32_t, uint32_t> Coord2D;

// instance
std::unique_ptr<NcursesViewer> NcursesViewer::instance_;

// resize handler
void NcursesViewer::NcursesViewerResizeHandler(int sig) noexcept {
    // the instance will always be ready to use
    // cannot cause data race
    NcursesViewer &instance = NcursesViewer::instance();
    // the original handler cannot cause race condition
    instance.saved_handler_(sig);
    // Redraw is synchronized
    instance.Redraw();
}

// constructor
NcursesViewer::NcursesViewer(void (*resize_handler)(int)) noexcept
        : redrawing_(false) {
    initscr();
    curs_set(0);
    noecho();
    cbreak();
    keypad(stdscr, TRUE);
    saved_handler_ = std::signal(SIGWINCH, resize_handler);
}

// destructor
NcursesViewer::~NcursesViewer() noexcept {
    std::signal(SIGWINCH, saved_handler_);
    endwin();
}

// update
void NcursesViewer::Update(const GameState &state) {
    saved_state_.reset(new GameState(state));
    Redraw();
}

// get dimension
void NcursesViewer::dimension(uint32_t &height,
                              uint32_t &width) const noexcept {
    unsigned int scr_height, scr_width;
    getmaxyx(stdscr, scr_height, scr_width);
    height = scr_height;
    width = scr_width;
}


// get status line
const std::string &NcursesViewer::GetStatusLine() const noexcept {
    return status_;
}

// set status line
void NcursesViewer::SetStatusLine(const std::string &status) noexcept {
    status_ = status;
    Redraw();
}
void NcursesViewer::SetStatusLine(std::string &&status) noexcept {
    status_ = std::move(status);
    Redraw();
}

namespace {

inline uint8_t GetMaxWidth(const GameState &state) noexcept {
    uint8_t max = 3;
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

// a simple lock guard for a bool flag
class bool_lock_guard {
 public:
    explicit bool_lock_guard(bool &lock) : lock_(lock) {
        lock_ = true;
    }
    ~bool_lock_guard() {
        lock_ = false;
    }
 private:
    bool &lock_;
};

}  // namespace

void NcursesViewer::Redraw() noexcept {
    // synchronize
    if (redrawing_)
        return;
    bool_lock_guard lock(redrawing_);

    // calculate size parameters
    unsigned int scr_height, scr_width;
    getmaxyx(stdscr, scr_height, scr_width);
    uint8_t max_width = GetMaxWidth(*saved_state_);
    Coord2D dim = GetDimension(saved_state_->height(),
                               saved_state_->width(), max_width);
    uint32_t top = scr_height < std::get<0>(dim) ?
            0 : (scr_height - std::get<0>(dim)) / 2;
    uint32_t left = scr_width < std::get<1>(dim) ?
            0 : (scr_width - std::get<1>(dim)) / 2;

    // actuate
    clear();
    attron(A_BOLD);
    for (uint32_t r = 0; r < saved_state_->height(); r++) {
        // draw upper boarder
        if (r == 0) {
            move(top, left);
            addch(ACS_ULCORNER);
            for (uint32_t c = 0; c < saved_state_->width(); c++) {
                for (uint8_t i = 0; i < max_width; i++)
                    addch(ACS_HLINE);
                addch(c + 1 == saved_state_->width() ? ACS_URCORNER : ACS_TTEE);
            }
        }

        // draw content
        move(top + r * 2 + 1, left);
        addch(ACS_VLINE);
        for (uint32_t c = 0; c < saved_state_->width(); c++) {
            if (saved_state_->tile(GameState::Position(r, c)).empty()) {
                for (uint8_t i = 0; i < max_width; i++)
                    addch(' ');
            } else {
                std::ostringstream oss;
                oss << std::setw(max_width)
                        << saved_state_->tile(GameState::Position(r, c));
                addstr(oss.str().c_str());
            }
            addch(ACS_VLINE);
        }

        // draw lower line
        move(top + r * 2 + 2, left);
        if (r + 1 == saved_state_->height()) {
            addch(ACS_LLCORNER);
            for (uint32_t c = 0; c < saved_state_->width(); c++) {
                for (uint8_t i = 0; i < max_width; i++)
                    addch(ACS_HLINE);
                addch(c + 1 == saved_state_->width() ? ACS_LRCORNER : ACS_BTEE);
            }
        } else {
            addch(ACS_LTEE);
            for (uint32_t c = 0; c < saved_state_->width(); c++) {
                for (uint8_t i = 0; i < max_width; i++)
                    addch(ACS_HLINE);
                addch(c + 1 == saved_state_->width() ? ACS_RTEE : ACS_PLUS);
            }
        }
    }
    attroff(A_BOLD);

    // draw status line
    move(scr_height - 1, 0);
    addstr(status_.c_str());

    refresh();
}

}  // namespace ui
}  // namespace _2048
