#include <gtest/gtest.h>
#include <sstream>
#include <exception>
#include "tile.h"

class TileTest : public testing::Test {
 protected:
    _2048::Tile tile_0_;
    _2048::Tile tile_2_;
    _2048::Tile tile_2048_;

    void SetUp() override {
        tile_0_ = _2048::Tile();
        tile_2_ = _2048::Tile(1);
        tile_2048_ = _2048::Tile(11);
    }
};

TEST_F(TileTest, Empty) {
    EXPECT_TRUE(tile_0_.empty());
    EXPECT_FALSE(tile_2_.empty());
    EXPECT_FALSE(tile_2048_.empty());
}

TEST_F(TileTest, PreIncrement) {
    EXPECT_THROW(++tile_0_, std::logic_error);
    EXPECT_EQ(++tile_2_, _2048::Tile(2));
    EXPECT_EQ(++tile_2048_, _2048::Tile(12));
}

TEST_F(TileTest, PostIncrement) {
    EXPECT_THROW(tile_0_++, std::logic_error);

    EXPECT_EQ(tile_2_++, _2048::Tile(1));
    EXPECT_EQ(tile_2_, _2048::Tile(2));

    EXPECT_EQ(tile_2048_++, _2048::Tile(11));
    EXPECT_EQ(tile_2048_, _2048::Tile(12));
}

TEST_F(TileTest, Print) {
    {
        std::ostringstream oss;
        oss << tile_0_;
        EXPECT_EQ(oss.str(), "");
    }
    {
        std::ostringstream oss;
        oss << tile_2_;
        EXPECT_EQ(oss.str(), "2");
    }
    {
        std::ostringstream oss;
        oss << tile_2048_;
        EXPECT_EQ(oss.str(), "2048");
    }
}
