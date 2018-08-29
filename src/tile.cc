#include <cstdint>
#include <ostream>
#include <exception>
#include "tile.h"

namespace _2048 {

const Tile Tile::kEmpty(0);

// post-increment
Tile Tile::operator++(int) {
    if (empty())
        throw std::logic_error("attempt to increase an empty tile");
    Tile original(*this);
    power_++;
    return original;
}

// pre-increment
Tile &Tile::operator++() {
    if (empty())
        throw std::logic_error("attempt to increase an empty tile");
    power_++;
    return *this;
}

// print
std::ostream &operator<<(std::ostream &os, const Tile &t) {
    if (!t.empty())
        os << (1ull << t.power());
    return os;
}

}  // namespace _2048
