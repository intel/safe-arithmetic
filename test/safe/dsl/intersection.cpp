#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_intersection, eval_ival_overlap) {
    EXPECT_EQ((constrain_interval<0, 20> && constrain_interval<10, 30>), (constrain_interval<10, 20>));
}

TEST(safe_dsl_intersection, eval_lhs_ival_contained) {
    EXPECT_EQ((constrain_interval<15, 25> && constrain_interval<10, 30>), (constrain_interval<15, 25>));
}

TEST(safe_dsl_intersection, eval_rhs_ival_contained) {
    EXPECT_EQ((constrain_interval<15, 25> && constrain_interval<19, 21>), (constrain_interval<19, 21>));
}

TEST(safe_dsl_intersection, eval_single_int_1) {
    EXPECT_EQ((constrain_interval<15, 25> && constrain_interval<25, 35>), (constrain_interval<25, 25>));
}

TEST(safe_dsl_intersection, eval_single_int_2) {
    EXPECT_EQ((constrain_interval<35, 50> && constrain_interval<25, 35>), (constrain_interval<35, 35>));
}

TEST(safe_dsl_intersection, eval_three_ival_overlap) {
    EXPECT_EQ((constrain_interval<0, 20> && constrain_interval<10, 30> && constrain_interval<-100, 15>), (constrain_interval<10, 15>));
}