#ifndef _NCURSES_VIEW_H_
#define _NCURSES_VIEW_H_

#include <memory>
#include "game_state.h"
#include "viewer.h"

namespace _2048 {

/**
 * A Ncurses based CLI viewer.
 */
class NcursesViewer : public Viewer {
 public:
    static NcursesViewer viewer;            /**< The NcursesViewer instance */
    void Update(const GameState &state) override;

 private:
    std::unique_ptr<GameState> saved_state_;  /**< The last updated state */
    void (*saved_handler_)(int);        /**< The original SIGWINCH handler */
    NcursesViewer() noexcept;                 /**< Default constructor */
    ~NcursesViewer() noexcept override;
    friend void ResizeHandler(int sig) noexcept;  /**< Handler for SIGWINCH */
};

}  // namespace _2048

#endif  // _NCURSES_VIEW_H_
