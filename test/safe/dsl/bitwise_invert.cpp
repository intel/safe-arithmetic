#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_bitwise_invert, mask_bitwise_invert) {
    EXPECT_EQ(~constrain_mask<0>, (constrain_mask<0, ~0>));
    EXPECT_EQ(~constrain_mask<0b1111>, (constrain_mask<0b1111, ~0>));
    EXPECT_NE(~constrain_mask<0>, constrain_mask<0>);
}