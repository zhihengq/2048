#ifndef _NCURSES_CONTROL_H_
#define _NCURSES_CONTROL_H_

#include <memory>
#include "game_state.h"
#include "ncurses_viewer.h"
#include "player.h"

namespace _2048 {

/**
 * A Ncurses based CLI viewer and controller.
 */
class NcursesController : public NcursesViewer, public Player {
 public:
    bool Play(const GameState &state, GameState::Direction *move) override;

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

    /** Constructor */
    explicit NcursesController(void (*resize_handler)(int)) noexcept
            : NcursesViewer(resize_handler) { }
};

}  // namespace _2048

#endif  // _NCURSES_CONTROL_H_
