#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

using namespace safe;
using namespace safe::literals;


TEST(safe_dsl_bitwise_and, mask_variable_bits_only) {
    EXPECT_EQ(mask<0b0> & mask<0b0>, mask<0b0>);
    EXPECT_EQ(mask<0b1> & mask<0b0>, mask<0b0>);
    EXPECT_EQ(mask<0b0> & mask<0b1>, mask<0b0>);
    EXPECT_EQ(mask<0b1> & mask<0b1>, mask<0b1>);

    EXPECT_EQ(mask<0b01> & mask<0b10>, mask<0b00>);
    EXPECT_EQ(mask<0b01> & mask<0b11>, mask<0b01>);
    EXPECT_EQ(mask<0b1110> & mask<0b0111>, mask<0b0110>);
}

TEST(safe_dsl_bitwise_and, constant_bits_only) {
    EXPECT_EQ((mask<0, 0b0> & mask<0, 0b0>), (mask<0, 0b0>));
    EXPECT_EQ((mask<0, 0b1> & mask<0, 0b0>), (mask<0, 0b0>));
    EXPECT_EQ((mask<0, 0b0> & mask<0, 0b1>), (mask<0, 0b0>));
    EXPECT_EQ((mask<0, 0b1> & mask<0, 0b1>), (mask<0, 0b1>));

    EXPECT_EQ((mask<0, 0xff00> & mask<0, 0xfeed>), (mask<0, 0xfe00>));
}

TEST(safe_dsl_bitwise_and, const_x_var_bits) {
    EXPECT_EQ((mask<0, 0> & mask<0, 0>), (mask<0, 0>));
    EXPECT_EQ((mask<0, 0> & mask<0, 1>), (mask<0, 0>));
    EXPECT_EQ((mask<0, 0> & mask<1, 0>), (mask<0, 0>));
    EXPECT_EQ((mask<0, 0> & mask<1, 1>), (mask<0, 0>));
    EXPECT_EQ((mask<0, 1> & mask<0, 0>), (mask<0, 0>));
    EXPECT_EQ((mask<0, 1> & mask<0, 1>), (mask<0, 1>));
    EXPECT_EQ((mask<0, 1> & mask<1, 0>), (mask<1, 0>));
    EXPECT_EQ((mask<0, 1> & mask<1, 1>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 0> & mask<0, 0>), (mask<0, 0>));
    EXPECT_EQ((mask<1, 0> & mask<0, 1>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 0> & mask<1, 0>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 0> & mask<1, 1>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 1> & mask<0, 0>), (mask<0, 0>));
    EXPECT_EQ((mask<1, 1> & mask<0, 1>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 1> & mask<1, 0>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 1> & mask<1, 1>), (mask<1, 0>));

    EXPECT_EQ((mask<0, 0xff> & mask<0xff>), (mask<0xff>));
    EXPECT_EQ((mask<0xff> & mask<0, 0xff>), (mask<0xff>));
}
