#ifndef _NCURSES_VIEW_H_
#define _NCURSES_VIEW_H_

#include <memory>
#include "game_state.h"
#include "viewer.h"

namespace _2048 {

class NcursesViewer : public Viewer {
 public:
    static NcursesViewer viewer;
    void Update(const GameState &state) override;
 private:
    std::unique_ptr<GameState> saved_state_;
    void (*saved_handler_)(int);
    NcursesViewer() noexcept;
    ~NcursesViewer() noexcept override;
    void Refresh() noexcept;
    friend void ResizeHandler(int sig) noexcept;
};

}  // namespace _2048

#endif  // _NCURSES_VIEW_H_
