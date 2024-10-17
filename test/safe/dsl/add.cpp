#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe/big_integer_gen.hpp>

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_add, add_two_ival_constants) {
    EXPECT_EQ((ival<30, 30> + ival<12, 12>), (ival<42, 42>));
}

TEST(safe_dsl_add, add_three_ival_constants) {
    EXPECT_EQ((ival<30, 30> + ival<12, 12> + ival<8, 8>), (ival<50, 50>));
}

TEST(safe_dsl_add, add_two_intervals) {
    EXPECT_EQ((ival<10, 20> + ival<40, 80>), (ival<50, 100>));
}

TEST(safe_dsl_add, add_three_intervals) {
    EXPECT_EQ((ival<10, 20> + ival<40, 80> + ival<1, 5>), (ival<51, 105>));
}

TEST(safe_dsl_add, add_four_intervals) {
    EXPECT_EQ((ival<10, 20> + ival<40, 80> + ival<1, 5> + ival<3, 7>),
              (ival<54, 112>));
}

TEST(safe_dsl_add, add_two_mask_constants) {
    EXPECT_EQ((mask<0, 12> + mask<0, 8>), (ival<20, 20>));
}

TEST(safe_dsl_add, add_two_masks_1) {
    auto const actual = dsl::detail::simp(mask<0x3> + mask<0x3>);

    EXPECT_EQ(actual.var_bits, 0x7);
}

TEST(safe_dsl_add, add_two_masks_2) {
    EXPECT_EQ((mask<0xf> + mask<0xff>), (mask<0x1ff>));
}

TEST(safe_dsl_add, add_two_masks_3) {
    EXPECT_EQ((mask<0, 13> + mask<0, 29>), (mask<0, 42>));
}

TEST(safe_dsl_add, add_two_masks_4) {
    EXPECT_EQ((mask<0x11> + mask<0x11>), (mask<0x33>));
}

TEST(safe_dsl_add, union_simplification) {
    EXPECT_EQ(((ival<0, 10> || ival<40, 80>)+ival<20, 30>),
              (ival<20, 40> || ival<60, 110>));

    EXPECT_EQ((ival<20, 30> + (ival<0, 10> || ival<40, 80>)),
              (ival<20, 40> || ival<60, 110>));

    EXPECT_EQ(
        ((ival<0, 10> || ival<100, 200>)+(ival<20, 30> || ival<400, 800>)),
        (ival<20, 40> || ival<120, 230> || ival<400, 1000>));
}

TEST(safe_dsl_add, hex_u8) {
    EXPECT_EQ((0x4_u8 + 0x2_u8 + 0x1_u8), (0x7_u8));
    EXPECT_EQ((0x3_u8 + 0x1_u8), (0x4_u8));
    EXPECT_EQ((0xCE_u8 + 0x0_u8), (0xCE_u8));
    EXPECT_EQ((0xFF_u8 + 0x0_u8), (0xFF_u8));
    EXPECT_EQ((0xAA_u8 + 0x55_u8), (0xFF_u8));
    EXPECT_EQ((0xf0_u8 + 0x0f_u8), (0xff_u8));
}