#include "grid.h"

#include <gtest/gtest.h>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "tile.h"

namespace {

using _2048::Grid;
using _2048::Tile;

class GridTest : public testing::Test {
 protected:
    std::unique_ptr<Grid> grid_1x2_;
    std::unique_ptr<Grid> grid_4x4_empty_;
    std::unique_ptr<Grid> grid_4x4_normal_;

    void SetUp() override {
        // 1x2
        // [[_, 2]]
        grid_1x2_.reset(new Grid(1, 2));
        grid_1x2_->tile(0, 1) = Tile(1);
        // 4x4 empty
        // [[_, _, _, _],
        //  [_, _, _, _],
        //  [_, _, _, _],
        //  [_, _, _, _]]
        grid_4x4_empty_.reset(new Grid(4, 4));
        // 4x4 normal
        // [[_,    2,  _,  4],
        //  [_,    2,  _,  _],
        //  [8,    4,  4,  _],
        //  [2048, 64, 32, _]]
        grid_4x4_normal_.reset(new Grid(4, 4));
        grid_4x4_normal_->tile(0, 1) = Tile(1);
        grid_4x4_normal_->tile(0, 3) = Tile(2);
        grid_4x4_normal_->tile(1, 1) = Tile(1);
        grid_4x4_normal_->tile(2, 0) = Tile(3);
        grid_4x4_normal_->tile(2, 1) = Tile(2);
        grid_4x4_normal_->tile(2, 2) = Tile(2);
        grid_4x4_normal_->tile(3, 0) = Tile(11);
        grid_4x4_normal_->tile(3, 1) = Tile(6);
        grid_4x4_normal_->tile(3, 2) = Tile(5);
    }
};

TEST_F(GridTest, TileOutOfRange) {
    EXPECT_THROW(grid_1x2_->tile(1, 0), std::out_of_range);
    EXPECT_THROW(grid_4x4_empty_->tile(2, 4), std::out_of_range);
    EXPECT_THROW(grid_4x4_normal_->tile(9, 10), std::out_of_range);
}

TEST_F(GridTest, ConstructEmpty) {
    EXPECT_THROW(Grid(0, 0), std::invalid_argument);
    EXPECT_THROW(Grid(0, 5), std::invalid_argument);
    EXPECT_THROW(Grid(2, 0), std::invalid_argument);
}

TEST_F(GridTest, CopyMoveConstruct) {
    Grid g1x2(*grid_1x2_);
    Grid g4x4e(*grid_4x4_empty_);
    Grid g4x4n(*grid_4x4_normal_);

    ASSERT_EQ(g1x2, *grid_1x2_);
    ASSERT_EQ(g4x4e, *grid_4x4_empty_);
    ASSERT_EQ(g4x4n, *grid_4x4_normal_);

    Grid mg1x2(std::move(*grid_1x2_));
    Grid mg4x4e(std::move(*grid_4x4_empty_));
    Grid mg4x4n(std::move(*grid_4x4_normal_));

    EXPECT_EQ(mg1x2, g1x2);
    EXPECT_EQ(mg4x4e, g4x4e);
    EXPECT_EQ(mg4x4n, g4x4n);

    EXPECT_THROW(g1x2 == *grid_1x2_, std::runtime_error);
    EXPECT_THROW(g4x4e == *grid_4x4_empty_, std::runtime_error);
    EXPECT_THROW(g4x4n == *grid_4x4_normal_, std::runtime_error);
}

TEST_F(GridTest, Print) {
    {
        std::ostringstream oss;
        oss << *grid_1x2_;
        EXPECT_EQ(oss.str(), "[[,2]]");
    }
    {
        std::ostringstream oss;
        oss << *grid_4x4_empty_;
        EXPECT_EQ(oss.str(), "[[,,,],[,,,],[,,,],[,,,]]");
    }
    {
        std::ostringstream oss;
        oss << *grid_4x4_normal_;
        EXPECT_EQ(oss.str(), "[[,2,,4],[,2,,],[8,4,4,],[2048,64,32,]]");
    }
}

}  // namespace
