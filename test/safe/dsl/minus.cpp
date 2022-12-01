#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

using namespace safe;
using namespace safe::literals;


TEST(safe_dsl_minus, sub_two_ival_constants) {
    EXPECT_EQ(
        (ival<30, 30> - ival<12, 12>),
        (ival<18, 18>)
    );
}

TEST(safe_dsl_minus, sub_two_intervals) {
    EXPECT_EQ(
        (ival<10, 20> - ival<40, 80>),
        (ival<-70, -20>)
    );
}

TEST(safe_dsl_minus, sub_two_mask_constants) {
    EXPECT_EQ(
        (mask<0, 12> - mask<0, 8>),
        (mask<0, 4>)
    );
}

TEST(safe_dsl_minus, sub_two_masks_1) {
    auto const actual = dsl::detail::simp(mask<0x3> - mask<0x3>);

    EXPECT_EQ(
        actual.var_bits,
        0xffff'ffff
    );
}

TEST(safe_dsl_minus, sub_two_masks_2) {
    EXPECT_EQ(
        (mask<0xf> - mask<0xff>),
        (mask<0xffff'ffff>)
    );
}

TEST(safe_dsl_minus, sub_two_masks_3) {
    EXPECT_EQ(
        (mask<0, 13> - mask<0, 29>),
        (mask<0, -16>)
    );
}

TEST(safe_dsl_minus, sub_two_masks_4) {
    EXPECT_EQ(
        (mask<0x11> - mask<0x11>),
        (mask<0xffff'ffff>)
    );
}
