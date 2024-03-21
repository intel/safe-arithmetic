#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_add, add_two_ival_constants) {
    EXPECT_EQ((constrain_interval<30, 30> + constrain_interval<12, 12>), (constrain_interval<42, 42>));
}

TEST(safe_dsl_add, add_three_ival_constants) {
    EXPECT_EQ((constrain_interval<30, 30> + constrain_interval<12, 12> + constrain_interval<8, 8>), (constrain_interval<50, 50>));
}

TEST(safe_dsl_add, add_two_intervals) {
    EXPECT_EQ((constrain_interval<10, 20> + constrain_interval<40, 80>), (constrain_interval<50, 100>));
}

TEST(safe_dsl_add, add_three_intervals) {
    EXPECT_EQ((constrain_interval<10, 20> + constrain_interval<40, 80> + constrain_interval<1, 5>), (constrain_interval<51, 105>));
}

TEST(safe_dsl_add, add_four_intervals) {
    EXPECT_EQ((constrain_interval<10, 20> + constrain_interval<40, 80> + constrain_interval<1, 5> + constrain_interval<3, 7>),
              (constrain_interval<54, 112>));
}

TEST(safe_dsl_add, add_two_mask_constants) {
    EXPECT_EQ((constrain_mask<0, 12> + constrain_mask<0, 8>), (constrain_interval<20, 20>));
}

TEST(safe_dsl_add, add_two_masks_1) {
    auto const actual = dsl::detail::simp(constrain_mask<0x3> + constrain_mask<0x3>);

    EXPECT_EQ(actual.var_bits, 0x7);
}

TEST(safe_dsl_add, add_two_masks_2) {
    EXPECT_EQ((constrain_mask<0xf> + constrain_mask<0xff>), (constrain_mask<0x1ff>));
}

TEST(safe_dsl_add, add_two_masks_3) {
    EXPECT_EQ((constrain_mask<0, 13> + constrain_mask<0, 29>), (constrain_mask<0, 42>));
}

TEST(safe_dsl_add, add_two_masks_4) {
    EXPECT_EQ((constrain_mask<0x11> + constrain_mask<0x11>), (constrain_mask<0x33>));
}

TEST(safe_dsl_add, union_simplification) {
    EXPECT_EQ(((constrain_interval<0, 10> || constrain_interval<40, 80>)+constrain_interval<20, 30>),
              (constrain_interval<20, 40> || constrain_interval<60, 110>));

    EXPECT_EQ((constrain_interval<20, 30> + (constrain_interval<0, 10> || constrain_interval<40, 80>)),
              (constrain_interval<20, 40> || constrain_interval<60, 110>));

    EXPECT_EQ(
        ((constrain_interval<0, 10> || constrain_interval<100, 200>)+(constrain_interval<20, 30> || constrain_interval<400, 800>)),
        (constrain_interval<20, 40> || constrain_interval<120, 230> || constrain_interval<400, 1000>));
}
