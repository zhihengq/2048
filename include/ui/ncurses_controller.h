#ifndef _UI_NCURSES_CONTROLLER_H_
#define _UI_NCURSES_CONTROLLER_H_

#include <memory>
#include <ncurses.h>

#include "game_state.h"
#include "player.h"
#include "ui/ncurses_viewer.h"

namespace _2048 {
namespace ui {

/**
 * A Ncurses based CLI viewer and controller.
 */
class NcursesController : public NcursesViewer, public Player {
 public:
    NcursesController(const NcursesController &) = delete;

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
            instance_.reset(new NcursesController(
                                NcursesControllerResizeHandler));
        return *instance_;
    }

 private:
    /** The NcursesController instance */
    static std::unique_ptr<NcursesController> instance_;
    int last_key_;  /**< The last key the user pressed */

    /** Constructor */
    explicit NcursesController(void (*resize_handler)(int)) noexcept
            : NcursesViewer(resize_handler), last_key_(ERR) { }

    /** Handler for SIGWINCH */
    static void NcursesControllerResizeHandler(int sig) noexcept;
};

}  // namespace ai
}  // namespace _2048

#endif  // _UI_NCURSES_CONTROLLER_H_
