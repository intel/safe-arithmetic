#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

using namespace safe;
using namespace safe::literals;



TEST(safe_dsl_intersection, simplify_ival_overlap) {
    EXPECT_EQ(
        (ival<0, 20> && ival<10, 30>),
        (ival<10, 20>)
    );
}

TEST(safe_dsl_intersection, simplify_lhs_ival_contained) {
    EXPECT_EQ(
        (ival<15, 25> && ival<10, 30>),
        (ival<15, 25>)
    );
}

TEST(safe_dsl_intersection, simplify_rhs_ival_contained) {
    EXPECT_EQ(
        (ival<15, 25> && ival<19, 21>),
        (ival<19, 21>)
    );
}

TEST(safe_dsl_intersection, simplify_single_int_1) {
    EXPECT_EQ(
        (ival<15, 25> && ival<25, 35>),
        (ival<25, 25>)
    );
}

TEST(safe_dsl_intersection, simplify_single_int_2) {
    EXPECT_EQ(
        (ival<35, 50> && ival<25, 35>),
        (ival<35, 35>)
    );
}

TEST(safe_dsl_intersection, simplify_three_ival_overlap) {
    EXPECT_EQ(
        (ival<0, 20> && ival<10, 30> && ival<-100, 15>),
        (ival<10, 15>)
    );
}