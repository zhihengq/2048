#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

#include <cstdint>
#include <ostream>
#include <vector>
#include "tile.h"
#include "grid.h"

namespace _2048 {

/**
 * Represents a game state.
 */
class GameState {
 public:
    friend std::ostream &operator<<(std::ostream &os, const GameState &g);
    GameState &operator=(const GameState &) = delete;

    /**
     * A struct packing a row index and a col index.
     */
    struct Position {
        uint32_t r;
        uint32_t c;
        explicit constexpr Position(uint32_t r = 0, uint32_t c = 0) noexcept
                : r(r), c(c) { }
        constexpr Position(const Position &p) noexcept : r(p.r), c(p.c) { }
        bool operator==(const Position &p) const noexcept {
            return r == p.r && c == p.c;
        }
    };

    /**
     * The four directions that each move can be in.
     */
    enum class Direction { UP, DOWN, LEFT, RIGHT };

    /**
     * Construct an empty `GameState` of size `height` by `width`.
     * @param height the height of the game grid
     * @param width the width of the game grid
     * @throw std::invalid_argument if the size if empty
     */
    GameState(uint32_t height, uint32_t width) : grid_(height, width) { }

    /**
     * Copy constructor
     * @param g the `GameState` to be copied
     * @throw std::runtime_error if `g` is not in a valid state
     */
    GameState(const GameState &g) : grid_(g.grid_) { }

    /**
     * Move constructor.
     * The original game state `g` will be unusable.
     * @param g the `GameState` to be moved
     */
    GameState(GameState &&g) noexcept : grid_(std::move(g.grid_)) { }

    /**
     * Get game grid height.
     * @return the height
     */
    uint32_t height() const noexcept { return grid_.height; }

    /**
     * Get game grid width.
     * @return the width
     */
    uint32_t width() const noexcept { return grid_.width; }

    /**
     * Equality
     * @param g the other `GameState` object
     * @return true if the two game states are identical, false otherwise
     * @throw std::runtime_error if `this` or `g` is not in a valid state
     */
    bool operator==(const GameState &g) const { return grid_ == g.grid_; }

    /**
     * Inequality
     * @param g the other `GameState` object
     * @return false if the two game states are identical, true otherwise
     * @throw std::runtime_error if `this` or `g` is not in a valid state
     */
    bool operator!=(const GameState &g) const { return grid_ != g.grid_; }

    /**
     * Get a tile.
     * @param pos the position of the tile
     * @return a read-only view of the tile
     * @throw std::out_of_range if the position is out of range
     * @throw std::runtime_error if `this` is not in a valid state
     */
    const Tile &tile(Position pos) const { return grid_.tile(pos.r, pos.c); }

    /**
     * Get a list of empty tiles.
     * Generator side operation.
     * @return a vector of pointers to available tiles in the game grid
     * @throw std::runtime_error if `this` is not in a valid state
     */
    std::vector<Position> GetEmptyTiles() const;

    /**
     * Generate a tile in the game grid.
     * Generator side operation.
     * @param pos the position of the new tile
     * @param power number in the new tile in terms of power of 2
     * @return true if the tile is generated, false if the position is not empty
     * @throw std::out_of_range if the position is out of range
     * @throw std::runtime_error if `this` is not in a valid state
     */
    bool GenerateTile(Position pos, uint8_t power);

    /**
     * Get the directions that can be moved in.
     * Player side operation.
     * @return a vector of directions that can be moved in
     * @throw std::runtime_error if `this` is not in a valid state
     */
    std::vector<Direction> GetPossibleMoves() const;

    /**
     * Apply a move.
     * Player side operation.
     * @param dir the direction
     * @return true if the move is applied, false if the move is not possible
     * @throw std::runtime_error if `this` is not in a valid state
     */
    bool Move(Direction dir);

 private:
    Grid grid_;  /**< Underlying grid */
    void GetPossibleDir(bool *left, bool *right,
                        bool *up, bool *down) const;
};

/**
 * Print the game grid as a human-readable string
 * @param os the output stream
 * @param g the `GameState` object
 * @return `os`
 * @throw std::runtime_error if `g` is not in a valid state
 */
inline std::ostream &operator<<(std::ostream &os, const GameState &g) {
    return os << g.grid_;
}

}  // namespace _2048

#endif  // _GAME_STATE_H_
