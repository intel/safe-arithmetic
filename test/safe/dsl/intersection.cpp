#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_intersection, eval_ival_overlap) {
    EXPECT_EQ((ival<0, 20> && ival<10, 30>), (ival<10, 20>));
}

TEST(safe_dsl_intersection, eval_lhs_ival_contained) {
    EXPECT_EQ((ival<15, 25> && ival<10, 30>), (ival<15, 25>));
}

TEST(safe_dsl_intersection, eval_rhs_ival_contained) {
    EXPECT_EQ((ival<15, 25> && ival<19, 21>), (ival<19, 21>));
}

TEST(safe_dsl_intersection, eval_single_int_1) {
    EXPECT_EQ((ival<15, 25> && ival<25, 35>), (ival<25, 25>));
}

TEST(safe_dsl_intersection, eval_single_int_2) {
    EXPECT_EQ((ival<35, 50> && ival<25, 35>), (ival<35, 35>));
}

TEST(safe_dsl_intersection, eval_three_ival_overlap) {
    EXPECT_EQ((ival<0, 20> && ival<10, 30> && ival<-100, 15>), (ival<10, 15>));
}