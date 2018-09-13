#include "ai/eval/weight_table/gradient_exponential_4x4.h"

#include <gtest/gtest.h>

#include "game_state.h"

namespace {

using _2048::GameState;

class GradientExponential4x4Test : public testing::Test {
 protected:
    std::unique_ptr<GameState> state_empty_;
    std::unique_ptr<GameState> state_normal_;
    _2048::ai::eval::weight_table::GradientExponential4x4 eval_;

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

TEST_F(GradientExponential4x4Test, Empty) {
    EXPECT_EQ(eval_(*state_empty_), 0);
}

TEST_F(GradientExponential4x4Test, Normal) {
    // 2048*1 + 64*2 + 8*2 + 32*4 + 4*4 + 4*8 + 2*8 + 2*16 + 4*64
    EXPECT_EQ(eval_(*state_normal_), -2672);
}

}  // namespace
