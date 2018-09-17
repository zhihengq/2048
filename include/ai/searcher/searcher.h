#ifndef _AI_SEARCHER_SEARCHER_H_
#define _AI_SEARCHER_SEARCHER_H_

#include <cstdint>

#include "ai/eval/eval_func.h"
#include "generator.h"
#include "player.h"

namespace _2048 {
namespace ai {
namespace searcher {

/**
 * Base class for all seachers.
 */
class Searcher : public Generator, public Player {
 public:
    /**
     * Initialize `Searcher` with a specific evaluation function and number of plies.
     * @param eval the evaluation function
     * @param ply the number of ply to search
     */
    explicit Searcher(const eval::EvaluationFunction &eval,
                      uint32_t ply = 0) noexcept : eval_(&eval), ply_(ply) { }

    /**
     * Set evaluation function.
     * @param eval the evaluation function
     * @return a reference to the original evaluation function
     */
    const eval::EvaluationFunction &SetEval(
            const eval::EvaluationFunction &eval) noexcept {
        const eval::EvaluationFunction &ret = *eval_;
        eval_ = &eval;
        return ret;
    }

    /**
     * Set number of plies.
     * @param ply the number of plies
     * @return the original number of plies
     */
    uint32_t SetPly(uint32_t ply) noexcept {
        uint32_t ret = ply_;
        ply_ = ply;
        return ret;
    }

 protected:
    // never nullptr
    const eval::EvaluationFunction *eval_;  /**< evaluation function */
    uint32_t ply_;                          /**< number of ply to search */
};

}  // namespace searcher
}  // namespace ai
}  // namespace _2048

#endif  // _AI_SEARCHER_SEARCHER_H_
