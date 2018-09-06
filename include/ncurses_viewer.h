#ifndef _NCURSES_VIEWER_H_
#define _NCURSES_VIEWER_H_

#include <memory>
#include "game_state.h"
#include "viewer.h"

namespace _2048 {

/**
 * A Ncurses based CLI viewer.
 */
class NcursesViewer : public Viewer {
 public:
    NcursesViewer(const NcursesViewer &) = delete;

    ~NcursesViewer() noexcept override;
    void Update(const GameState &state) override;

    /**
     * Get the instance of NcursesViewer.
     * @return the instance
     */
    static NcursesViewer &instance() noexcept {
        if (instance_ == nullptr)
            instance_.reset(new NcursesViewer(NcursesViewerResizeHandler));
        return *instance_;
    }

 protected:
    /** The NcursesViewer instance */
    static std::unique_ptr<NcursesViewer> instance_;

    std::unique_ptr<GameState> saved_state_;    /**< The last updated state */
    void (*saved_handler_)(int);        /**< The original SIGWINCH handler */

    /** Constructor */
    explicit NcursesViewer(void (*resize_handler)(int)) noexcept;

    /** Handler for SIGWINCH */
    static void NcursesViewerResizeHandler(int sig) noexcept;
};

}  // namespace _2048

#endif  // _NCURSES_VIEWER_H_
