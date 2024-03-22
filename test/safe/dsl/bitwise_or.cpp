#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_bitwise_or, mask_variable_bits_only) {
    EXPECT_EQ(constrain_mask<0b0> | constrain_mask<0b0>, constrain_mask<0b0>);
    EXPECT_EQ(constrain_mask<0b1> | constrain_mask<0b0>, constrain_mask<0b1>);
    EXPECT_EQ(constrain_mask<0b0> | constrain_mask<0b1>, constrain_mask<0b1>);
    EXPECT_EQ(constrain_mask<0b1> | constrain_mask<0b1>, constrain_mask<0b1>);

    EXPECT_EQ(constrain_mask<0b01> | constrain_mask<0b10>, constrain_mask<0b11>);

    EXPECT_EQ(constrain_mask<0xba5e0000> | constrain_mask<0x0000ba11>, constrain_mask<0xba5eba11>);
}

TEST(safe_dsl_bitwise_or, constant_bits_only) {
    EXPECT_EQ((constrain_mask<0, 0> | constrain_mask<0, 0>), (constrain_mask<0, 0>));
    EXPECT_EQ((constrain_mask<0, 1> | constrain_mask<0, 0>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<0, 0> | constrain_mask<0, 1>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<0, 1> | constrain_mask<0, 1>), (constrain_mask<0, 1>));

    EXPECT_EQ((constrain_mask<0, 0xff00> | constrain_mask<0, 0xfeed>), (constrain_mask<0, 0xffed>));
}

TEST(safe_dsl_bitwise_or, const_x_var_bits) {
    EXPECT_EQ((constrain_mask<0, 0> | constrain_mask<0, 0>), (constrain_mask<0, 0>));
    EXPECT_EQ((constrain_mask<0, 0> | constrain_mask<0, 1>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<0, 0> | constrain_mask<1, 0>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<0, 0> | constrain_mask<1, 1>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<0, 1> | constrain_mask<0, 0>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<0, 1> | constrain_mask<0, 1>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<0, 1> | constrain_mask<1, 0>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<0, 1> | constrain_mask<1, 1>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<1, 0> | constrain_mask<0, 0>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 0> | constrain_mask<0, 1>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<1, 0> | constrain_mask<1, 0>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 0> | constrain_mask<1, 1>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 1> | constrain_mask<0, 0>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 1> | constrain_mask<0, 1>), (constrain_mask<0, 1>));
    EXPECT_EQ((constrain_mask<1, 1> | constrain_mask<1, 0>), (constrain_mask<1, 0>));
    EXPECT_EQ((constrain_mask<1, 1> | constrain_mask<1, 1>), (constrain_mask<1, 0>));

    EXPECT_EQ((constrain_mask<0, 0xff> | constrain_mask<0xff>), (constrain_mask<0, 0xff>));
    EXPECT_EQ((constrain_mask<0xff> | constrain_mask<0, 0xff>), (constrain_mask<0, 0xff>));
}