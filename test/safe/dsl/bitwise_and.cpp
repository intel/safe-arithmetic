#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_bitwise_and, mask_variable_bits_only) {
    EXPECT_EQ(constrain_mask<0b0> & constrain_mask<0b0>, constrain_mask<0b0>);
    EXPECT_EQ(constrain_mask<0b1> & constrain_mask<0b0>, constrain_mask<0b0>);
    EXPECT_EQ(constrain_mask<0b0> & constrain_mask<0b1>, constrain_mask<0b0>);
    EXPECT_EQ(constrain_mask<0b1> & constrain_mask<0b1>, constrain_mask<0b1>);

    EXPECT_EQ(constrain_mask<0b01> & constrain_mask<0b10>, constrain_mask<0b00>);
    EXPECT_EQ(constrain_mask<0b01> & constrain_mask<0b11>, constrain_mask<0b01>);
    EXPECT_EQ(constrain_mask<0b1110> & constrain_mask<0b0111>, constrain_mask<0b0110>);
}

TEST(safe_dsl_bitwise_and, constant_bits_only) {
    EXPECT_EQ((constrain_mask<0, 0b0> & constrain_mask<0, 0b0>), (constrain_mask<0, 0b0>));
    EXPECT_EQ((constrain_mask<0, 0b1> & constrain_mask<0, 0b0>), (constrain_mask<0, 0b0>));
    EXPECT_EQ((constrain_mask<0, 0b0> & constrain_mask<0, 0b1>), (constrain_mask<0, 0b0>));
    EXPECT_EQ((constrain_mask<0, 0b1> & constrain_mask<0, 0b1>), (constrain_mask<0, 0b1>));

    EXPECT_EQ((constrain_mask<0, 0xff00> & constrain_mask<0, 0xfeed>), (constrain_mask<0, 0xfe00>));
}

TEST(safe_dsl_bitwise_and, const_x_var_bits) {
    EXPECT_EQ((constrain_mask<0, 0> & constrain_mask<0, 0>), (constrain_mask<0, 0>));
    EXPECT_EQ((constrain_mask<0, 0> & constrain_mask<0, 1>), (constrain_mask<0, 0>));
    EXPECT_EQ((constrain_mask<0, 0> & constrain_mask<1, 0>), (constrain_mask<0, 0>));
    EXPECT_EQ((constrain_mask<0, 0> & constrain_mask<1, 1>), (constrain_mask<0, 0>));
    EXPECT_EQ((constrain_mask<0, 1> & constrain_mask<0, 0>), (constrain_mask<0, 0>));
    EXPECT_EQ((constrain_mask<0, 1> & constrain_mask<0, 1>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<0, 1> & constrain_mask<1, 0>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<0, 1> & constrain_mask<1, 1>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 0> & constrain_mask<0, 0>), (constrain_mask<0, 0>));
    EXPECT_EQ((constrain_mask<1, 0> & constrain_mask<0, 1>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 0> & constrain_mask<1, 0>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 0> & constrain_mask<1, 1>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 1> & constrain_mask<0, 0>), (constrain_mask<0, 0>));
    EXPECT_EQ((constrain_mask<1, 1> & constrain_mask<0, 1>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 1> & constrain_mask<1, 0>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 1> & constrain_mask<1, 1>), (constrain_mask<1, 0>));

    EXPECT_EQ((constrain_mask<0, 0xff> & constrain_mask<0xff>), (constrain_mask<0xff>));
    EXPECT_EQ((constrain_mask<0xff> & constrain_mask<0, 0xff>), (constrain_mask<0xff>));
}
