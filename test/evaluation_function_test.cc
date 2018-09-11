#include "ai/evaluation_function.h"

#include <cstdint>
#include <gtest/gtest.h>

namespace {

TEST(EvaluationFunctionTest, MaxMinConstants) {
    int64_t max = _2048::ai::EvaluationFunction::kMaxValue;
    int64_t min = _2048::ai::EvaluationFunction::kMinValue;
    EXPECT_EQ(max, 0x7FFFFFFFFFFFFFFF);
    EXPECT_EQ(min, -0x7FFFFFFFFFFFFFFF);
    EXPECT_EQ(max + min, 0);
}

}  // namespace
