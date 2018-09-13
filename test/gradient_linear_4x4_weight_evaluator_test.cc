#include "ai/gradient_linear_4x4_weight_evaluator.h"

#include <gtest/gtest.h>

#include "game_state.h"

namespace {

using _2048::GameState;

class GradientLinear4x4WeightEvaluatorTest : public testing::Test {
 protected:
    std::unique_ptr<GameState> state_empty_;
    std::unique_ptr<GameState> state_normal_;
    _2048::ai::GradientLinear4x4WeightEvaluator eval_;

    void SetUp() override {
        state_empty_.reset(new GameState(4, 4));
        // [[_,    2,  _,  4],
        //  [_,    2,  _,  _],
        //  [8,    4,  4,  _],
        //  [2048, 64, 32, _]]
        state_normal_.reset(new GameState(4, 4));
        state_normal_->GenerateTile(GameState::Position(0, 1), 1);
        state_normal_->GenerateTile(GameState::Position(0, 3), 2);
        state_normal_->GenerateTile(GameState::Position(1, 1), 1);
        state_normal_->GenerateTile(GameState::Position(2, 0), 3);
        state_normal_->GenerateTile(GameState::Position(2, 1), 2);
        state_normal_->GenerateTile(GameState::Position(2, 2), 2);
        state_normal_->GenerateTile(GameState::Position(3, 0), 11);
        state_normal_->GenerateTile(GameState::Position(3, 1), 6);
        state_normal_->GenerateTile(GameState::Position(3, 2), 5);
    }
};

TEST_F(GradientLinear4x4WeightEvaluatorTest, Empty) {
    EXPECT_EQ(eval_(*state_empty_), 0);
}

TEST_F(GradientLinear4x4WeightEvaluatorTest, Normal) {
    // 2048*1 + 64*2 + 8*2 + 32*3 + 4*3 + 4*4 + 2*4 + 2*5 + 4*7
    EXPECT_EQ(eval_(*state_normal_), -2362);
}

}  // namespace