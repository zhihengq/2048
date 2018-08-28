#include <cstdint>
#include <memory>
#include <ostream>
#include <exception>
#include "grid.h"

namespace _2048 {

// constructor
Grid::Grid(uint32_t height, uint32_t width) : height(height), width(width) {
    if (height * width == 0)
        throw std::invalid_argument("empty board size");
    grid_.reset(new Tile[height * width]);
}

// copy constructor
Grid::Grid(const Grid &g) : Grid(g.height, g.width) {
    if (g.grid_ == nullptr)
        throw std::runtime_error("invalid grid state");
    for (uint32_t i = 0; i < height * width; i++)
        grid_[i] = g.grid_[i];
}

// move constructor
Grid::Grid(Grid &&g) : height(g.height), width(g.width) {
    if (g.grid_ == nullptr)
        throw std::runtime_error("invalid grid state");
    grid_ = std::move(g.grid_);
}

// equality
bool Grid::operator==(const Grid &g) const {
    if (grid_ == nullptr || g.grid_ == nullptr)
        throw std::runtime_error("invalid grid state");
    if (height != g.height || width != g.width)
        return false;
    for (uint32_t i = 0; i < height * width; i++)
        if (grid_[i] != g.grid_[i])
            return false;
    return true;
}

// const accessor
const Tile &Grid::tile(uint32_t r, uint32_t c) const {
    if (grid_ == nullptr)
        throw std::runtime_error("invalid grid state");
    if (r >= height || c >= width)
        throw std::out_of_range("row or col index out of range");
    return grid_[offset(r, c)];
}

// accessor
Tile &Grid::tile(uint32_t r, uint32_t c) {
    if (grid_ == nullptr)
        throw std::runtime_error("invalid grid state");
    if (r >= height || c >= width)
        throw std::out_of_range("row or col index out of range");
    return grid_[offset(r, c)];
}

// print
std::ostream &operator<<(std::ostream &os, const Grid &g) {
    if (g.grid_ == nullptr)
        throw std::runtime_error("invalid grid state");
    os << '[';
    for (uint32_t i = 0; i < g.height; i++) {
        if (i != 0)
            os << ',';
        os << '[';
        for (uint32_t j = 0; j < g.width; j++) {
            if (j != 0)
                os << ',';
            os << g.tile(i, j);
        }
        os << ']';
    }
    os << ']';
    return os;
}

}  // namespace _2048
