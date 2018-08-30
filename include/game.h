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
     * Get a pointer to the viewer.
     * @return the viewer, or `nullptr` if there is none
     */
    Viewer *GetViewer() const noexcept { return viewer_; }

    /**
     * Set the viewer and return a pointer to the original viewer.
     * If `viewer` is null, then there will be no more viewer in this game.
     * @param viewer the new viewer
     * @return the old viewer, or `nullptr` if there is none
     */
    Viewer *SetViewer(Viewer *viewer) noexcept {
        Viewer *old_viewer = viewer_;
        viewer_ = viewer;
        return old_viewer;
    }

    /**
     * Get a pointer to the tile generator.
     * @return the generator, or `nullptr` if there is none
     */
    Generator *GetGenerator() const noexcept { return generator_; }

    /**
     * Set the generator and return a pointer to the original generator.
     * If `generator` is null, then there will be no more generator in this game.
     * @param generator the new generator
     * @return the old generator, or `nullptr` if there is none
     */
    Generator *SetGenerator(Generator *generator) noexcept {
        Generator *old_generator = generator_;
        generator_ = generator;
        return old_generator;
    }

    /**
     * Get a pointer to the player.
     * @return the player, or `nullptr` if there is none
     */
    Player *GetPlayer() const noexcept { return player_; }

    /**
     * Set the player and return a pointer to the original player.
     * If `player` is null, then there will be no more player in this game.
     * @param player the new player
     * @return the old player, or `nullptr` if there is none
     */
    Player *SetPlayer(Player *player) noexcept {
        Player *old_player = player_;
        player_ = player;
        return old_player;
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
    virtual ~Game();

 protected:
    GameState state_;
    Viewer    *viewer_;
    Generator *generator_;
    Player    *player_;
};

}  // namespace _2048

#endif  // _GAME_H_
