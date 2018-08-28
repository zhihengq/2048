#ifndef _GRID_H_
#define _GRID_H_

#include <cstdint>
#include <memory>
#include "tile.h"

namespace _2048 {

/**
 * @brief A grid of `Tile` representing the game board.
 */
class Grid {
 public:
    const uint32_t height;  /**< the grid height */
    const uint32_t width;   /**< the grid width */

    Grid &operator=(const Grid &) = delete;
    friend std::ostream &operator<<(std::ostream &os, const Grid &g);

    /**
     * Construct a `Grid` of size `height` by `width`.
     * The constructed grid will be all empty.
     * @param height the number of rows
     * @param width the number of columns
     * @throws std::invalid_argument if `height` or `width` is zero
     * @throws std::bad_alloc if memory allocation failed
     */
    Grid(uint32_t height, uint32_t width);

    /**
     * Copy constructor
     * @param g the `Grid` to be copied
     * @throws std::bad_alloc if memory allocation failed
     * @throws std::runtime_error if `g` is not in a valid state
     */
    Grid(const Grid &g);

    /**
     * Move constructor
     * @param g the `Grid` to be moved
     * @throws std::runtime_error if `g` is not in a valid state
     */
    Grid(Grid &&g);

    /**
     * Equality
     * @param g the other `Grid` object
     * @return true if the two grids are identical, false otherwise
     * @throws std::runtime_error if `this` or `g` is not in a valid state
     */
    bool operator==(const Grid &g) const;

    /**
     * Inequality
     * @param g the other `Grid` object
     * @return false if the two grids are identical, true otherwise
     * @throws std::runtime_error if `this` or `g` is not in a valid state
     */
    bool operator!=(const Grid &g) const { return !(*this == g); }

    //@{
    /**
     * Get a tile in this grid.
     * @param r the row index
     * @param c the col index
     * @return a reference to the tile
     * @throw std::out_of_range if `r` or `c` is out of range
     * @throws std::runtime_error if this grid is not in a valid state
     */
    const Tile &tile(uint32_t r, uint32_t c) const;
    Tile &tile(uint32_t r, uint32_t c);
    //@}

 private:
    std::unique_ptr<Tile[]> grid_;

    /**
     * Translate row and col into offset in `grid_`.
     * @param r the row index
     * @param c the col index
     * @pre `r` and `c` are in range
     * @return the offset
     */
    size_t offset(uint32_t r, uint32_t c) const { return r * width + c; }
};

/**
 * Print the grid as a human-readable string.
 * @param os the output stream
 * @param g the `Grid` object
 */
std::ostream &operator<<(std::ostream &os, const Grid &g);

}  // namespace _2048

#endif  // _GRID_H_
