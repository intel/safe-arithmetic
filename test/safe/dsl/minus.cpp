#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_minus, sub_two_ival_constants) {
    EXPECT_EQ((constrain_interval<30, 30> - constrain_interval<12, 12>), (constrain_interval<18, 18>));
}

TEST(safe_dsl_minus, sub_two_intervals) {
    EXPECT_EQ((constrain_interval<10, 20> - constrain_interval<40, 80>), (constrain_interval<-70, -20>));
}

TEST(safe_dsl_minus, sub_two_mask_constants) {
    EXPECT_EQ((constrain_mask<0, 12> - constrain_mask<0, 8>), (constrain_mask<0, 4>));
}

// TEST(safe_dsl_minus, sub_two_masks_1) {
//     auto const actual = dsl::detail::simp(constrain_mask<0x3> - constrain_mask<0x3>);

//     EXPECT_EQ(
//         actual.var_bits,
//         0xffff'ffff
//     );
// }

// TEST(safe_dsl_minus, sub_two_masks_2) {
//     EXPECT_EQ(
//         (constrain_mask<0xf> - constrain_mask<0xff>),
//         (constrain_mask<0xffff'ffff>)
//     );
// }

// TEST(safe_dsl_minus, sub_two_masks_3) {
//     EXPECT_EQ(
//         (constrain_mask<0, 13> - constrain_mask<0, 29>),
//         (constrain_mask<0, -16>)
//     );
// }

// TEST(safe_dsl_minus, sub_two_masks_4) {
//     EXPECT_EQ(
//         (constrain_mask<0x11> - constrain_mask<0x11>),
//         (constrain_mask<0xffff'ffff>)
//     );
// }
