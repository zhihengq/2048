#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <exception>
#include "tile.h"
#include "game_state.h"

using _2048::GameState;

class GameStateTest : public testing::Test {
 protected:
    std::unique_ptr<GameState> g1_;
    std::unique_ptr<GameState> g2_;

    void SetUp() override {
        // 2x2
        // [[2, 4 ],
        //  [8, 16]]
        g1_.reset(new GameState(2, 2));
        g1_->GenerateTile(GameState::Position(0, 0), 1);
        g1_->GenerateTile(GameState::Position(0, 1), 2);
        g1_->GenerateTile(GameState::Position(1, 0), 3);
        g1_->GenerateTile(GameState::Position(1, 1), 4);
        // 4x4
        // [[_,    2,  _,  4],
        //  [_,    2,  _,  _],
        //  [8,    4,  4,  _],
        //  [2048, 64, 32, _]]
        g2_.reset(new GameState(4, 4));
        g2_->GenerateTile(GameState::Position(0, 1), 1);
        g2_->GenerateTile(GameState::Position(0, 3), 2);
        g2_->GenerateTile(GameState::Position(1, 1), 1);
        g2_->GenerateTile(GameState::Position(2, 0), 3);
        g2_->GenerateTile(GameState::Position(2, 1), 2);
        g2_->GenerateTile(GameState::Position(2, 2), 2);
        g2_->GenerateTile(GameState::Position(3, 0), 11);
        g2_->GenerateTile(GameState::Position(3, 1), 6);
        g2_->GenerateTile(GameState::Position(3, 2), 5);
    }
};

TEST_F(GameStateTest, CopyMoveConstruct) {
    GameState g1(*g1_);
    GameState g2(*g2_);

    ASSERT_EQ(g1, *g1_);
    ASSERT_EQ(g2, *g2_);

    GameState mg1(std::move(*g1_));
    GameState mg2(std::move(*g2_));

    EXPECT_EQ(mg1, g1);
    EXPECT_EQ(mg2, g2);

    EXPECT_THROW(g1 == *g1_, std::runtime_error);
    EXPECT_THROW(g2 == *g2_, std::runtime_error);
}

TEST_F(GameStateTest, InvalidState) {
    GameState g(std::move(*g1_));

    EXPECT_THROW((GameState(*g1_)), std::runtime_error);
    EXPECT_THROW(g == *g1_, std::runtime_error);
    EXPECT_THROW(*g1_ == g, std::runtime_error);
    EXPECT_THROW(g != *g1_, std::runtime_error);
    EXPECT_THROW(*g1_ != g, std::runtime_error);
    EXPECT_THROW(g1_->tile(GameState::Position(0, 0)), std::runtime_error);
    EXPECT_THROW(g1_->GetEmptyTiles(), std::runtime_error);
    EXPECT_THROW(g1_->GenerateTile(GameState::Position(0, 0), 1),
                 std::runtime_error);
    EXPECT_THROW(g1_->GetPossibleMoves(), std::runtime_error);
    EXPECT_THROW(g1_->Move(GameState::Direction::LEFT), std::runtime_error);
}

TEST_F(GameStateTest, GetEmptyTiles) {
    auto e1 = g1_->GetEmptyTiles();
    auto e2 = g2_->GetEmptyTiles();

    EXPECT_EQ(e1, decltype(e1)({}));
    EXPECT_EQ(e2, decltype(e2)({
            GameState::Position(0, 0),
            GameState::Position(0, 2),
            GameState::Position(1, 0),
            GameState::Position(1, 2),
            GameState::Position(1, 3),
            GameState::Position(2, 3),
            GameState::Position(3, 3)
        }));
}

TEST_F(GameStateTest, GenerateTile) {
    EXPECT_THROW(g1_->GenerateTile(GameState::Position(2, 2), 1),
                 std::out_of_range);
    EXPECT_THROW(g1_->GenerateTile(GameState::Position(0, 0), 1),
                 std::invalid_argument);
    ASSERT_EQ(g2_->tile(GameState::Position(0, 2)).power(), 0);
    g2_->GenerateTile(GameState::Position(0, 2), 11);
    EXPECT_EQ(g2_->tile(GameState::Position(0, 2)).power(), 11);
}

TEST_F(GameStateTest, GetPossibleMoves) {
    auto m1 = g1_->GetPossibleMoves();
    auto m2 = g2_->GetPossibleMoves();
    EXPECT_EQ(m1, decltype(m1)({}));
    EXPECT_EQ(m2, decltype(m2)({
            GameState::Direction::LEFT,
            GameState::Direction::RIGHT,
            GameState::Direction::UP,
            GameState::Direction::DOWN
        }));
}

TEST_F(GameStateTest, MoveLeft) {
    GameState ans1(*g1_);
    EXPECT_FALSE(g1_->Move(GameState::Direction::LEFT));
    EXPECT_EQ(*g1_, ans1);

    // [[_,    2,  _,  4],     [[2,    4,  _,  _],
    //  [_,    2,  _,  _],  =>  [2,    _,  _,  _],
    //  [8,    4,  4,  _],      [8,    8,  _,  _],
    //  [2048, 64, 32, _]]      [2048, 64, 32, _]]
    GameState ans2(g2_->height(), g2_->width());
    ans2.GenerateTile(GameState::Position(0, 0), 1);
    ans2.GenerateTile(GameState::Position(0, 1), 2);
    ans2.GenerateTile(GameState::Position(1, 0), 1);
    ans2.GenerateTile(GameState::Position(2, 0), 3);
    ans2.GenerateTile(GameState::Position(2, 1), 3);
    ans2.GenerateTile(GameState::Position(3, 0), 11);
    ans2.GenerateTile(GameState::Position(3, 1), 6);
    ans2.GenerateTile(GameState::Position(3, 2), 5);
    EXPECT_TRUE(g2_->Move(GameState::Direction::LEFT));
    EXPECT_EQ(*g2_, ans2);
}

TEST_F(GameStateTest, MoveRight) {
    GameState ans1(*g1_);
    EXPECT_FALSE(g1_->Move(GameState::Direction::RIGHT));
    EXPECT_EQ(*g1_, ans1);

    // [[_,    2,  _,  4],     [[_, _,    2,  4 ],
    //  [_,    2,  _,  _],  =>  [_, _,    _,  2 ],
    //  [8,    4,  4,  _],      [_, _,    8,  8 ],
    //  [2048, 64, 32, _]]      [_, 2048, 64, 32]]
    GameState ans2(g2_->height(), g2_->width());
    ans2.GenerateTile(GameState::Position(0, 2), 1);
    ans2.GenerateTile(GameState::Position(0, 3), 2);
    ans2.GenerateTile(GameState::Position(1, 3), 1);
    ans2.GenerateTile(GameState::Position(2, 2), 3);
    ans2.GenerateTile(GameState::Position(2, 3), 3);
    ans2.GenerateTile(GameState::Position(3, 1), 11);
    ans2.GenerateTile(GameState::Position(3, 2), 6);
    ans2.GenerateTile(GameState::Position(3, 3), 5);
    EXPECT_TRUE(g2_->Move(GameState::Direction::RIGHT));
    EXPECT_EQ(*g2_, ans2);
}

TEST_F(GameStateTest, MoveUp) {
    GameState ans1(*g1_);
    EXPECT_FALSE(g1_->Move(GameState::Direction::UP));
    EXPECT_EQ(*g1_, ans1);

    // [[_,    2,  _,  4],     [[8,    4,  4,  4],
    //  [_,    2,  _,  _],  =>  [2048, 4,  32, _],
    //  [8,    4,  4,  _],      [_,    64, _,  _],
    //  [2048, 64, 32, _]]      [_,    _,  _,  _]]
    GameState ans2(g2_->height(), g2_->width());
    ans2.GenerateTile(GameState::Position(0, 0), 3);
    ans2.GenerateTile(GameState::Position(0, 1), 2);
    ans2.GenerateTile(GameState::Position(0, 2), 2);
    ans2.GenerateTile(GameState::Position(0, 3), 2);
    ans2.GenerateTile(GameState::Position(1, 0), 11);
    ans2.GenerateTile(GameState::Position(1, 1), 2);
    ans2.GenerateTile(GameState::Position(1, 2), 5);
    ans2.GenerateTile(GameState::Position(2, 1), 6);
    EXPECT_TRUE(g2_->Move(GameState::Direction::UP));
    EXPECT_EQ(*g2_, ans2);
}

TEST_F(GameStateTest, MoveDown) {
    GameState ans1(*g1_);
    EXPECT_FALSE(g1_->Move(GameState::Direction::DOWN));
    EXPECT_EQ(*g1_, ans1);

    // [[_,    2,  _,  4],     [[_,    _,  _,  _],
    //  [_,    2,  _,  _],  =>  [_,    4,  _,  _],
    //  [8,    4,  4,  _],      [8,    4,  4,  _],
    //  [2048, 64, 32, _]]      [2048, 64, 32, 4]]
    GameState ans2(g2_->height(), g2_->width());
    ans2.GenerateTile(GameState::Position(1, 1), 2);
    ans2.GenerateTile(GameState::Position(2, 0), 3);
    ans2.GenerateTile(GameState::Position(2, 1), 2);
    ans2.GenerateTile(GameState::Position(2, 2), 2);
    ans2.GenerateTile(GameState::Position(3, 0), 11);
    ans2.GenerateTile(GameState::Position(3, 1), 6);
    ans2.GenerateTile(GameState::Position(3, 2), 5);
    ans2.GenerateTile(GameState::Position(3, 3), 2);
    EXPECT_TRUE(g2_->Move(GameState::Direction::DOWN));
    EXPECT_EQ(*g2_, ans2);
}

TEST_F(GameStateTest, Print) {
    {
        std::ostringstream oss;
        oss << *g1_;
        EXPECT_EQ(oss.str(), "[[2,4],[8,16]]");
    }
    {
        std::ostringstream oss;
        oss << *g2_;
        EXPECT_EQ(oss.str(), "[[,2,,4],[,2,,],[8,4,4,],[2048,64,32,]]");
    }
}
