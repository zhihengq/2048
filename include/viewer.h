#ifndef _VIEWER_H_
#define _VIEWER_H_

#include "game_state.h"

namespace _2048 {

/**
 * Interface of a game viewer that recieve updates from a `Game`.
 */
class Viewer {
 public:
    /**
     * Update the viewer based on the game state.
     * @param state the new game state
     */
    virtual void Update(const GameState &state) = 0;

    /**
     * Destructor
     */
    virtual ~Viewer() { }
};

}  // namespace _2048

#endif  // _VIEWER_H_
