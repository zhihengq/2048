#include "ai/eval/eval_func.h"

#include <cstdint>
#include <gtest/gtest.h>

namespace {

TEST(EvaluationFunctionTest, MaxMinConstants) {
    constexpr int64_t max = _2048::ai::eval::EvaluationFunction::kPosInf;
    constexpr int64_t min = _2048::ai::eval::EvaluationFunction::kNegInf;
    constexpr int64_t over = _2048::ai::eval::EvaluationFunction::kOver;
    EXPECT_EQ(max, 0x7FFFFFFFFFFFFFFF);
    EXPECT_EQ(min, -0x8000000000000000);
    EXPECT_EQ(over, -1000000000000000000);
}

}  // namespace
