#include <cstdint>
#include <vector>
#include "tile.h"
#include "game_state.h"

namespace _2048 {

// get empty tiles
std::vector<GameState::Position> GameState::GetEmptyTiles() const {
    std::vector<GameState::Position> empties;
    for (uint32_t r = 0; r < height(); r++)
        for (uint32_t c = 0; c < width(); c++)
            if (tile(Position(r, c)).empty())
                empties.emplace_back(r, c);
    return empties;
}

// generate tile
void GameState::GenerateTile(GameState::Position pos, uint8_t power) {
    if (!tile(pos).empty())
        throw std::invalid_argument(
                "Cannot generate tiles on non-empty position");
    grid_.tile(pos.r, pos.c) = Tile(power);
}

// get possible move directions
void GameState::GetPossibleDir(bool *left, bool *right,
                               bool *up, bool *down) const {
    bool left_possible  = false;
    bool right_possible = false;
    bool up_possible    = false;
    bool down_possible  = false;

    // check left/right
    if (left != nullptr || right != nullptr) {
        for (uint32_t r = 0; r < height(); r++)
            for (uint32_t c = 0; c + 1 < width(); c++)
                if (grid_.tile(r, c).empty()) {
                    if (grid_.tile(r, c + 1).empty()) {
                        // both empty
                        // cannot move, do nothing
                    } else {
                        // left empty, right occupied
                        left_possible = true;
                    }
                } else {
                    if (grid_.tile(r, c + 1).empty()) {
                        // left occupied, right empty
                        right_possible = true;
                    } else {
                        // both occupied
                        if (grid_.tile(r, c) == grid_.tile(r, c + 1))
                            left_possible = right_possible = true;
                    }
                }
    }

    // check up/down
    if (up != nullptr || down != nullptr) {
        for (uint32_t r = 0; r + 1 < height(); r++)
            for (uint32_t c = 0; c < width(); c++)
                if (grid_.tile(r, c).empty()) {
                    if (grid_.tile(r + 1, c).empty()) {
                        // both empty
                        // cannot move, do nothing
                    } else {
                        // up empty, down occupied
                        up_possible = true;
                    }
                } else {
                    if (grid_.tile(r, c + 1).empty()) {
                        // up occupied, down empty
                        down_possible = true;
                    } else {
                        // both occupied
                        if (grid_.tile(r, c) == grid_.tile(r + 1, c))
                            up_possible = down_possible = true;
                    }
                }
    }

    // collect output
    if (left  != nullptr) *left  = left_possible;
    if (right != nullptr) *right = right_possible;
    if (up    != nullptr) *up    = up_possible;
    if (down  != nullptr) *down  = down_possible;
}

// get possible moves
std::vector<GameState::Direction> GameState::GetPossibleMoves() const {
    bool left, right, up, down;
    std::vector<GameState::Direction> directions;

    GetPossibleDir(&left, &right, &up, &down);
    if (left)  directions.push_back(GameState::Direction::LEFT);
    if (right) directions.push_back(GameState::Direction::RIGHT);
    if (up)    directions.push_back(GameState::Direction::UP);
    if (down)  directions.push_back(GameState::Direction::DOWN);

    return directions;
}

// apply move
bool GameState::Move(GameState::Direction dir) {
    // check if move is possible
    bool possible;
    switch (dir) {
        case Direction::LEFT:
            GetPossibleDir(&possible, nullptr, nullptr, nullptr);
            break;
        case Direction::RIGHT:
            GetPossibleDir(nullptr, &possible, nullptr, nullptr);
            break;
        case Direction::UP:
            GetPossibleDir(nullptr, nullptr, &possible, nullptr);
            break;
        case Direction::DOWN:
            GetPossibleDir(nullptr, nullptr, nullptr, &possible);
            break;
    }
    if (!possible) return false;

    switch (dir) {
        case Direction::LEFT:
            for (uint32_t r = 0; r < height(); r++)
                for (uint32_t c0 = 1, c1 = 0; c0 < width(); c0++) {
                    if (grid_.tile(r, c0).empty()) {
                        continue;
                    } else if (grid_.tile(r, c1).empty()) {
                        grid_.tile(r, c1) = grid_.tile(r, c0);
                    } else if (grid_.tile(r, c1) == grid_.tile(r, c0)) {
                        ++grid_.tile(r, c1++);
                    } else {
                        grid_.tile(r, ++c1) = grid_.tile(r, c0);
                        if (c0 == c1) continue;
                    }
                    grid_.tile(r, c0) = Tile::kEmpty;
                }
            break;
        case Direction::RIGHT:
            for (uint32_t r = 0; r < height(); r++)
                for (uint32_t c0 = width() - 2, c1 = width() - 1;
                        c0 < width(); c0--) {
                    if (grid_.tile(r, c0).empty()) {
                        continue;
                    } else if (grid_.tile(r, c1).empty()) {
                        grid_.tile(r, c1) = grid_.tile(r, c0);
                    } else if (grid_.tile(r, c1) == grid_.tile(r, c0)) {
                        ++grid_.tile(r, c1--);
                    } else {
                        grid_.tile(r, --c1) = grid_.tile(r, c0);
                        if (c0 == c1) continue;
                    }
                    grid_.tile(r, c0) = Tile::kEmpty;
                }
            break;
        case Direction::UP:
            for (uint32_t c = 0; c < width(); c++)
                for (uint32_t r0 = 1, r1 = 0;
                        r0 < height(); r0++) {
                    if (grid_.tile(r0, c).empty()) {
                        continue;
                    } else if (grid_.tile(r1, c).empty()) {
                        grid_.tile(r1, c) = grid_.tile(r0, c);
                    } else if (grid_.tile(r1, c) == grid_.tile(r0, c)) {
                        ++grid_.tile(r1++, c);
                    } else {
                        grid_.tile(++r1, c) = grid_.tile(r0, c);
                        if (r0 == r1) continue;
                    }
                    grid_.tile(r0, c) = Tile::kEmpty;
                }
            break;
        case Direction::DOWN:
            for (uint32_t c = 0; c < width(); c++)
                for (uint32_t r0 = height() - 2, r1 = height() - 1;
                        r0 < height(); r0--) {
                    if (grid_.tile(r0, c).empty()) {
                        continue;
                    } else if (grid_.tile(r1, c).empty()) {
                        grid_.tile(r1, c) = grid_.tile(r0, c);
                    } else if (grid_.tile(r1, c) == grid_.tile(r0, c)) {
                        ++grid_.tile(r1--, c);
                    } else {
                        grid_.tile(--r1, c) = grid_.tile(r0, c);
                        if (r0 == r1) continue;
                    }
                    grid_.tile(r0, c) = Tile::kEmpty;
                }
            break;
    }
    return true;
}

}  // namespace _2048
