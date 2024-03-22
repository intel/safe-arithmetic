#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_is_subset, superset_v_subset_op) {
    EXPECT_TRUE(!(constrain_interval<0, 100> >= constrain_interval<0, 101>));
    EXPECT_TRUE(!(constrain_interval<0, 100> >= constrain_interval<200, 300>));
    EXPECT_TRUE(!(constrain_interval<0, 100> <= constrain_interval<200, 300>));
    EXPECT_TRUE(!(constrain_interval<0, 200> >= constrain_interval<100, 300>));
    EXPECT_TRUE(!(constrain_interval<0, 200> <= constrain_interval<100, 300>));
}

TEST(safe_dsl_is_subset, superset_op) {
    EXPECT_TRUE((constrain_interval<0, 100> >= constrain_interval<10, 90>));
    EXPECT_TRUE((constrain_interval<0, 100> >= constrain_interval<0, 100>));
}

TEST(safe_dsl_is_subset, superset_union_simplification) {
    EXPECT_TRUE((constrain_interval<0, 100> >= (constrain_interval<10, 20> || constrain_interval<30, 40>)));
    EXPECT_TRUE(((constrain_interval<0, 100> || constrain_interval<200, 300>) >= constrain_interval<10, 20>));
    EXPECT_TRUE(((constrain_interval<0, 100> || constrain_interval<200, 300>) >= constrain_interval<250, 300>));
    EXPECT_TRUE(
        ((constrain_interval<0, 100> || constrain_interval<200, 300>) >= (constrain_interval<10, 20> || constrain_interval<250, 300>)));
}