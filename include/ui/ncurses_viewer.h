#ifndef _UI_NCURSESVIEWER_H_
#define _UI_NCURSESVIEWER_H_

#include <string>
#include <memory>

#include "game_state.h"
#include "viewer.h"

namespace _2048 {
namespace ui {

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

    /**
     * Get the terminal dimension.
     * @param height output the height
     * @param width output the width
     */
    void dimension(uint32_t &height, uint32_t &width) const noexcept;

    /**
     * Get the status line string.
     * @return the status line string
     */
    virtual const std::string &GetStatusLine() const noexcept;

    /**
     * Set the status line string.
     * @param status the status line string
     */
    //@{
    virtual void SetStatusLine(const std::string &status) noexcept;
    virtual void SetStatusLine(std::string &&status) noexcept;
    //@}

 protected:
    /** The NcursesViewer instance */
    static std::unique_ptr<NcursesViewer> instance_;

    std::unique_ptr<GameState> saved_state_;    /**< The last updated state */
    std::string status_;                        /**< The status line string */
    void (*saved_handler_)(int);        /**< The original SIGWINCH handler */
    bool redrawing_;                            /**< Locked during redrawing */

    /** Constructor */
    explicit NcursesViewer(void (*resize_handler)(int)) noexcept;

    /** Handler for SIGWINCH */
    static void NcursesViewerResizeHandler(int sig) noexcept;

    /** Redraw the screen */
    virtual void Redraw() noexcept;
};

}  // namespace ui
}  // namespace _2048

#endif  // _UI_NCURSESVIEWER_H_
