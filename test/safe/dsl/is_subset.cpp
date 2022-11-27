#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

using namespace safe;
using namespace safe::literals;





TEST(safe_dsl_is_subset, superset_v_subset_op) {
    EXPECT_TRUE(!(ival<0, 100> >= ival<0, 101>));
    EXPECT_TRUE(!(ival<0, 100> >= ival<200, 300>));
    EXPECT_TRUE(!(ival<0, 100> <= ival<200, 300>));
    EXPECT_TRUE(!(ival<0, 200> >= ival<100, 300>));
    EXPECT_TRUE(!(ival<0, 200> <= ival<100, 300>));
}

TEST(safe_dsl_is_subset, superset_op) {
    EXPECT_TRUE((ival<0, 100> >= ival<10, 90>));
    EXPECT_TRUE((ival<0, 100> >= ival<0, 100>));
}

TEST(safe_dsl_is_subset, superset_union_simplification) {
    EXPECT_TRUE((ival<0, 100> >= (ival<10, 20> || ival<30, 40>)));
    EXPECT_TRUE(((ival<0, 100> || ival<200, 300>) >= ival<10, 20>));
    EXPECT_TRUE(((ival<0, 100> || ival<200, 300>) >= ival<250, 300>));
    EXPECT_TRUE(((ival<0, 100> || ival<200, 300>) >= (ival<10, 20> || ival<250, 300>)));
}