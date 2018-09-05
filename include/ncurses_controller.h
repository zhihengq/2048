#ifndef _NCURSES_CONTROL_H_
#define _NCURSES_CONTROL_H_

#include <memory>
#include <ncurses.h>
#include "game_state.h"
#include "ncurses_viewer.h"
#include "player.h"

namespace _2048 {

/**
 * A Ncurses based CLI viewer and controller.
 */
class NcursesController : public NcursesViewer, public Player {
 public:
    /**
     * Ask the user for a move.
     * The function will fail and return false if the user pressed a key other
     * than the arrow keys.
     * @copydoc
     */
    bool Play(const GameState &state, GameState::Direction *move) override;

    /**
     * Get the last key the user pressed.
     * @return the last keypress, or ERR if no key has been pressed
     */
    int GetLastKey() const noexcept { return last_key_; }

    /**
     * Get the instance of NcursesViewer.
     * @return the instance
     */
    static NcursesController &instance() noexcept {
        if (instance_ == nullptr)
            instance_.reset(new NcursesController(NcursesViewerResizeHandler));
        return *instance_;
    }

 private:
    /** The NcursesController instance */
    static std::unique_ptr<NcursesController> instance_;
    int last_key_;  /**< The last key the user pressed */

    /** Constructor */
    explicit NcursesController(void (*resize_handler)(int)) noexcept
            : NcursesViewer(resize_handler), last_key_(ERR) { }
};

}  // namespace _2048

#endif  // _NCURSES_CONTROL_H_
