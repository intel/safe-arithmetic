#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

using namespace safe;
using namespace safe::literals;



TEST(safe_dsl_bitwise_invert, mask_bitwise_invert) {
    EXPECT_EQ(~mask<0>, (mask<0, ~0u>));
    EXPECT_EQ(~mask<0b1111>, (mask<0b1111, ~0u>));

    EXPECT_EQ(~mask<0> | mask<0>, (mask<0, ~0u>));


    EXPECT_NE(~mask<0>, mask<0>);
}