#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_is_equal, simple_ivals) {
    EXPECT_EQ((constrain_interval<10, 20>), (constrain_interval<10, 20>));

    EXPECT_NE((constrain_interval<10, 20>), (constrain_interval<10, 10>));
}

TEST(safe_dsl_is_equal, union_ivals) {
    EXPECT_EQ((constrain_interval<10, 20> || constrain_interval<40, 50>), (constrain_interval<10, 20> || constrain_interval<40, 50>));

    EXPECT_EQ((constrain_interval<10, 20> || constrain_interval<40, 50>), (constrain_interval<40, 50> || constrain_interval<10, 20>));
}

TEST(safe_dsl_is_equal, differing_ival_types) {
    EXPECT_EQ((constrain_interval<10, 20>), (constrain_interval<10u, 20u>));

    EXPECT_EQ((constrain_interval<10ll, 20ll>), (constrain_interval<10, 20>));
}
