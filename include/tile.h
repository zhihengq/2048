#ifndef _TILE_H_
#define _TILE_H_

#include <cstdint>
#include <ostream>

namespace _2048 {

/**
 * A single tile in the game.
 * A tile is empty or contains a number which is a power of 2.
 */
class Tile {
 public:
    static const Tile kEmpty;  /**< Empty tile */

    /**
     * Copy constructor
     * @param t the `Tile` to be copied
     */
    Tile(const Tile &t) : power_(t.power_) { }

    /**
     * Construct a `Tile` with a initial number.
     * If the initial number is 1 (power is 0), the tile will be empty.
     * @param power an exponent n such that the initial number is 2^n
     */
    explicit Tile(uint8_t power = 0) : power_(power) { }

    /**
     * Copy assignment
     * @return `*this`
     */
    Tile &operator=(const Tile &t) { power_ = t.power_; return *this; }

    /**
     * Get the number in this tile in terms of power of 2.
     * @return the power n such that 2^n is the number in this tile,
     *          or 0 if this tile contains no number;
     */
    uint8_t power() const { return power_; }

    /**
     * Test whether this tile contains a number.
     * @return true if this tile is empty, false otherwise
     */
    bool empty() const { return power() == 0; }

    /**
     * Equality
     * @return true if the two tiles contain the same number or are both empty,
     *         false otherwise
     */
    bool operator==(const Tile &t) const { return power() == t.power(); }

    /**
     * Inequality
     * @return false if the two tiles contain the same number or are both empty,
     *         true otherwise
     */
    bool operator!=(const Tile &t) const { return !(*this == t); }

    /**
     * Doubles the number in this tile if this tile contains a number.
     * @return a copy of the original tile
     * @throws std::logic_error if this tile is empty
     */
    Tile operator++(int);

    /**
     * Doubles the number in this tile if this tile contains a number.
     * @return *this
     * @throws std::logic_error if this tile is empty
     */
    Tile &operator++();

 private:
    uint8_t power_;
};

/**
 * Print the number in this tile.
 * @param os the output stream
 * @param t the `Tile` object
 */
std::ostream &operator<<(std::ostream &os, const Tile &t);

}  // namespace _2048

#endif  // _TILE_H_
