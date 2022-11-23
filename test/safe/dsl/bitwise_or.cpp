#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

using namespace safe;
using namespace safe::literals;



TEST(safe_dsl_bitwise_or, mask_variable_bits_only) {
    EXPECT_EQ(mask<0b0> | mask<0b0>, mask<0b0>);
    EXPECT_EQ(mask<0b1> | mask<0b0>, mask<0b1>);
    EXPECT_EQ(mask<0b0> | mask<0b1>, mask<0b1>);
    EXPECT_EQ(mask<0b1> | mask<0b1>, mask<0b1>);

    EXPECT_EQ(mask<0b01> | mask<0b10>, mask<0b11>);

    EXPECT_EQ(mask<0xba5e0000> | mask<0x0000ba11>, mask<0xba5eba11>);
}

TEST(safe_dsl_bitwise_or, constant_bits_only) {
    EXPECT_EQ((mask<0, 0> | mask<0, 0>), (mask<0, 0>));
    EXPECT_EQ((mask<0, 1> | mask<0, 0>), (mask<0, 1>));
    EXPECT_EQ((mask<0, 0> | mask<0, 1>), (mask<0, 1>));
    EXPECT_EQ((mask<0, 1> | mask<0, 1>), (mask<0, 1>));

    EXPECT_EQ((mask<0, 0xff00> | mask<0, 0xfeed>), (mask<0, 0xffed>));
}


TEST(safe_dsl_bitwise_or, const_x_var_bits) {
    EXPECT_EQ((mask<0, 0> | mask<0, 0>), (mask<0, 0>));
    EXPECT_EQ((mask<0, 0> | mask<0, 1>), (mask<0, 1>));
    EXPECT_EQ((mask<0, 0> | mask<1, 0>), (mask<1, 0>));
    EXPECT_EQ((mask<0, 0> | mask<1, 1>), (mask<1, 0>));
    EXPECT_EQ((mask<0, 1> | mask<0, 0>), (mask<0, 1>));
    EXPECT_EQ((mask<0, 1> | mask<0, 1>), (mask<0, 1>));
    EXPECT_EQ((mask<0, 1> | mask<1, 0>), (mask<0, 1>));
    EXPECT_EQ((mask<0, 1> | mask<1, 1>), (mask<0, 1>));
    EXPECT_EQ((mask<1, 0> | mask<0, 0>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 0> | mask<0, 1>), (mask<0, 1>));
    EXPECT_EQ((mask<1, 0> | mask<1, 0>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 0> | mask<1, 1>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 1> | mask<0, 0>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 1> | mask<0, 1>), (mask<0, 1>));
    EXPECT_EQ((mask<1, 1> | mask<1, 0>), (mask<1, 0>));
    EXPECT_EQ((mask<1, 1> | mask<1, 1>), (mask<1, 0>));

    EXPECT_EQ((mask<0, 0xff> | mask<0xff>), (mask<0, 0xff>));
    EXPECT_EQ((mask<0xff> | mask<0, 0xff>), (mask<0, 0xff>));
}