#include "game.h"

#include <gtest/gtest.h>
#include <cstdint>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "game_state.h"
#include "viewer.h"
#include "generator.h"
#include "player.h"

namespace {

using _2048::Game;
using _2048::GameState;

class DummyViewer : public _2048::Viewer {
 public:
    std::unique_ptr<const GameState> state_;
    void Update(const GameState &state) override {
        state_.reset(new GameState(state));
    }
};

class DummyGenerator : public _2048::Generator {
 public:
    bool Generate(const GameState &state, GameState::Position *pos,
                  uint8_t *power) override {
        *pos = GameState::Position(0, 0);
        *power = 1;
        return true;
    }
};

class DummyPlayer : public _2048::Player {
 public:
    virtual bool Play(const GameState &state, GameState::Direction *move) {
        *move = GameState::Direction::LEFT;
        return true;
    }
};

class GameTest : public testing::Test {
 protected:
    Game g1_;
    Game g2_;
    DummyViewer    view1_;
    DummyViewer    view2_;
    DummyGenerator gene_;
    DummyPlayer    play_;

    void SetUp() override {
        // 2x2
        // [[2, 4 ],
        //  [8, 16]]
        GameState s1(2, 2);
        s1.GenerateTile(GameState::Position(0, 0), 1);
        s1.GenerateTile(GameState::Position(0, 1), 2);
        s1.GenerateTile(GameState::Position(1, 0), 3);
        s1.GenerateTile(GameState::Position(1, 1), 4);
        g1_.SetViewer(&view1_);
        g1_.SetGenerator(&gene_);
        g1_.SetPlayer(&play_);
        g1_.Reset(std::move(s1));
        // 4x4
        // [[_,    2,  _,  4],
        //  [_,    2,  _,  _],
        //  [8,    4,  4,  _],
        //  [2048, 64, 32, _]]
        GameState s2(4, 4);
        s2.GenerateTile(GameState::Position(0, 1), 1);
        s2.GenerateTile(GameState::Position(0, 3), 2);
        s2.GenerateTile(GameState::Position(1, 1), 1);
        s2.GenerateTile(GameState::Position(2, 0), 3);
        s2.GenerateTile(GameState::Position(2, 1), 2);
        s2.GenerateTile(GameState::Position(2, 2), 2);
        s2.GenerateTile(GameState::Position(3, 0), 11);
        s2.GenerateTile(GameState::Position(3, 1), 6);
        s2.GenerateTile(GameState::Position(3, 2), 5);
        g2_.SetViewer(&view2_);
        g2_.SetGenerator(&gene_);
        g2_.SetPlayer(&play_);
        g2_.Reset(std::move(s2));
    }
};

TEST_F(GameTest, MoveConstruct) {
    Game g1(std::move(g1_));
    Game g2(std::move(g2_));

    EXPECT_EQ(g1.GetViewer(),    &view1_);
    EXPECT_EQ(g2.GetViewer(),    &view2_);
    EXPECT_EQ(g1.GetGenerator(), &gene_);
    EXPECT_EQ(g2.GetGenerator(), &gene_);
    EXPECT_EQ(g1.GetPlayer(),    &play_);
    EXPECT_EQ(g2.GetPlayer(),    &play_);
}

TEST_F(GameTest, InvalidState) {
    Game g1(std::move(g1_));
    Game g2(std::move(g2_));

    EXPECT_THROW(g1_.Generate(), std::runtime_error);
    EXPECT_THROW(g2_.Generate(), std::runtime_error);
    EXPECT_THROW(g1_.Play(), std::runtime_error);
    EXPECT_THROW(g2_.Play(), std::runtime_error);
}

TEST_F(GameTest, GetClients) {
    EXPECT_EQ(g1_.GetViewer(),    &view1_);
    EXPECT_EQ(g2_.GetViewer(),    &view2_);
    EXPECT_EQ(g1_.GetGenerator(), &gene_);
    EXPECT_EQ(g2_.GetGenerator(), &gene_);
    EXPECT_EQ(g1_.GetPlayer(),    &play_);
    EXPECT_EQ(g2_.GetPlayer(),    &play_);
}

TEST_F(GameTest, SetClients) {
    EXPECT_EQ(g1_.SetViewer(nullptr), &view1_);
    EXPECT_EQ(g1_.SetViewer(&view1_), nullptr);
    EXPECT_EQ(g1_.GetViewer(), &view1_);

    EXPECT_EQ(g1_.SetGenerator(nullptr), &gene_);
    EXPECT_EQ(g1_.SetGenerator(&gene_), nullptr);
    EXPECT_EQ(g1_.GetGenerator(), &gene_);

    EXPECT_EQ(g1_.SetPlayer(nullptr), &play_);
    EXPECT_EQ(g1_.SetPlayer(&play_), nullptr);
    EXPECT_EQ(g1_.GetPlayer(), &play_);
}

TEST_F(GameTest, Generate) {
    EXPECT_FALSE(g1_.Generate());
    EXPECT_TRUE(g2_.Generate());

    // 2x2
    // [[2, 4 ],
    //  [8, 16]]
    GameState s1(2, 2);
    s1.GenerateTile(GameState::Position(0, 0), 1);
    s1.GenerateTile(GameState::Position(0, 1), 2);
    s1.GenerateTile(GameState::Position(1, 0), 3);
    s1.GenerateTile(GameState::Position(1, 1), 4);
    // 4x4
    // [[2,    2,  _,  4],
    //  [_,    2,  _,  _],
    //  [8,    4,  4,  _],
    //  [2048, 64, 32, _]]
    GameState s2(4, 4);
    s2.GenerateTile(GameState::Position(0, 0), 1);
    s2.GenerateTile(GameState::Position(0, 1), 1);
    s2.GenerateTile(GameState::Position(0, 3), 2);
    s2.GenerateTile(GameState::Position(1, 1), 1);
    s2.GenerateTile(GameState::Position(2, 0), 3);
    s2.GenerateTile(GameState::Position(2, 1), 2);
    s2.GenerateTile(GameState::Position(2, 2), 2);
    s2.GenerateTile(GameState::Position(3, 0), 11);
    s2.GenerateTile(GameState::Position(3, 1), 6);
    s2.GenerateTile(GameState::Position(3, 2), 5);

    ASSERT_NE(view1_.state_, nullptr);
    EXPECT_EQ(*view1_.state_, s1);

    ASSERT_NE(view2_.state_, nullptr);
    EXPECT_EQ(*view2_.state_, s2);
}

TEST_F(GameTest, Play) {
    EXPECT_FALSE(g1_.Play());
    EXPECT_TRUE(g2_.Play());

    // 2x2
    // [[2, 4 ],
    //  [8, 16]]
    GameState s1(2, 2);
    s1.GenerateTile(GameState::Position(0, 0), 1);
    s1.GenerateTile(GameState::Position(0, 1), 2);
    s1.GenerateTile(GameState::Position(1, 0), 3);
    s1.GenerateTile(GameState::Position(1, 1), 4);
    // 4x4
    // [[2,    4,  _,  _],
    //  [2,    _,  _,  _],
    //  [8,    8,  _,  _],
    //  [2048, 64, 32, _]]
    GameState s2(4, 4);
    s2.GenerateTile(GameState::Position(0, 0), 1);
    s2.GenerateTile(GameState::Position(0, 1), 2);
    s2.GenerateTile(GameState::Position(1, 0), 1);
    s2.GenerateTile(GameState::Position(2, 0), 3);
    s2.GenerateTile(GameState::Position(2, 1), 3);
    s2.GenerateTile(GameState::Position(3, 0), 11);
    s2.GenerateTile(GameState::Position(3, 1), 6);
    s2.GenerateTile(GameState::Position(3, 2), 5);

    ASSERT_NE(view1_.state_, nullptr);
    EXPECT_EQ(*view1_.state_, s1);

    ASSERT_NE(view2_.state_, nullptr);
    EXPECT_EQ(*view2_.state_, s2);
}

}  // namespace
