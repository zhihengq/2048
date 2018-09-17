#include "ai/searcher/minimax.h"

#include <cstdint>
#include <vector>
#include <stdexcept>

#include "ai/eval/eval_func.h"

namespace _2048 {
namespace ai {
namespace searcher {

namespace {
int64_t MaxGeneratorValue(const GameState &state,
                          const eval::EvaluationFunction &eval,
                          uint32_t ply,
                          GameState::Position *pos = nullptr,
                          uint8_t *power = nullptr);
int64_t MaxPlayerValue(const GameState &state,
                       const eval::EvaluationFunction &eval,
                       uint32_t ply,
                       GameState::Direction *move = nullptr);
}  // namespace

// generate
bool Minimax::Generate(const GameState &state, GameState::Position *pos,
                       uint8_t *power) {
    const auto v = state.GetEmptyTiles();
    if (v.empty())
        return false;
    if (ply_ == 0)
        *pos = v.front(), *power = 1;
    else
        MaxGeneratorValue(state, *eval_, ply_, pos, power);
    return true;
}

// play
bool Minimax::Play(const GameState &state, GameState::Direction *move) {
    const auto v = state.GetPossibleMoves();
    if (v.empty())
        return false;

    if (ply_ == 0)
        *move = v.front();
    else
        MaxPlayerValue(state, *eval_, ply_, move);

    return true;
}

namespace {

// minimax generator
int64_t MaxGeneratorValue(const GameState &state,
                          const eval::EvaluationFunction &eval,
                          uint32_t ply,
                          GameState::Position *pos,
                          uint8_t *power) {
    if (ply == 0)
        return -eval(state);    // negative player-value

    const auto tiles = state.GetEmptyTiles();
    if (tiles.empty())
        return -MaxPlayerValue(state, eval, ply - 1);

    int64_t value = eval::EvaluationFunction::kNegInf;
    for (const auto &t : tiles) {
        for (uint8_t p = 1; p <= 2; p++) {
            GameState next_state(state);
            if (!next_state.GenerateTile(t, p)) throw std::runtime_error(
                    "Tile returned by GetEmptyTile() cannot be generated");
            int64_t next_value = -MaxPlayerValue(next_state, eval, ply - 1);
            if (value < next_value) {
                value = next_value;
                if (pos != nullptr)
                    *pos = t;
                if (power != nullptr)
                    *power = p;
            }
        }
    }
    return value;
}

// minimax player
int64_t MaxPlayerValue(const GameState &state,
                       const eval::EvaluationFunction &eval,
                       uint32_t ply,
                       GameState::Direction *move) {
    if (ply == 0)
        return eval(state);     // positive player-value

    const auto moves = state.GetPossibleMoves();
    if (moves.empty())
        return eval::EvaluationFunction::kOver - ply;

    int64_t value = eval::EvaluationFunction::kNegInf;
    for (const auto &m : moves) {
        GameState next_state(state);
        if (!next_state.Move(m)) throw std::runtime_error(
                "Move returned by GetPossibleMoves() cannot be applied");
        int64_t next_value = -MaxGeneratorValue(next_state, eval, ply - 1);
        if (value < next_value) {
            value = next_value;
            if (move != nullptr)
                *move = m;
        }
    }
    return value;
}

}  // namespace

}  // namespace searcher
}  // namespace ai
}  // namespace _2048
