#ifndef _AI_SEARCHER_MINIMAX_H_
#define _AI_SEARCHER_MINIMAX_H_

#include <cstdint>

#include "ai/searcher/searcher.h"

namespace _2048 {
namespace ai {
namespace searcher {

/**
 * A basic minimax searcher
 */
class Minimax : public Searcher {
 public:
    bool Generate(const GameState &state, GameState::Position *pos,
                  uint8_t *power) override;

    bool Play(const GameState &state, GameState::Direction *move) override;
};

}  // namespace searcher
}  // namespace ai
}  // namespace _2048

#endif  // _AI_SEARCHER_MINIMAX_H_
