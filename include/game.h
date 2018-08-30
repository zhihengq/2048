#ifndef _GAME_H_
#define _GAME_H_

#include <cstdint>
#include "game_state.h"
#include "viewer/viewer.h"
#include "generator/generator.h"
#include "player/player.h"

namespace _2048 {

class Game {
 public:
    Game(const Game &) = delete;
    Game *operator=(const Game &) = delete;

    /**
     * Construct a game instance of size `height` by `width`.
     * @param height the height of the game grid
     * @param width the width of the game grid
     * @param viewer the viewer of the game
     * @param generator the tile generator
     * @param player the game player
     * @throw std::invalid_argument if the grid size is zero
     */
    Game(uint32_t height, uint32_t width,
         Viewer *viewer = nullptr,
         Generator *generator = nullptr,
         Player *player = nullptr)
            : state_(height, width), viewer_(viewer),
              generator_(generator), player_(player) { }

    /**
     * Move constructor.
     * The original game `g` will be unusable.
     * @param g the `Game` to be moved
     */
    Game(Game &&g) noexcept
            : state_(std::move(g.state_)),
              viewer_(g.viewer_),
              generator_(g.generator_),
              player_(g.player_) {
        g.viewer_    = nullptr;
        g.generator_ = nullptr;
        g.player_    = nullptr;
    }

    /**
     * Let the generator generate a tile.
     * If there is no generator, the operation will fail.
     * If there is a viewer, it will be updated.
     * @return true if successful, otherwise false
     */
    virtual bool Generate();

    /**
     * Let the player play a move.
     * If there is no player, the operation will fail.
     * If there is a viewer, it will be updated.
     * @return true if successful, otherwise false
     */
    virtual bool Play();

    /**
     * Destructor
     */
    virtual ~Game() { }

 protected:
    GameState state_;
    Viewer    *viewer_;
    Generator *generator_;
    Player    *player_;
};

}  // namespace _2048

#endif  // _GAME_H_
