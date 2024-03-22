#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_divide, divide_two_ival_constants) {
    EXPECT_EQ((constrain_interval<48, 48> / constrain_interval<4, 4>), (constrain_interval<12, 12>));
}

TEST(safe_dsl_divide, add_two_intervals) {
    EXPECT_EQ((constrain_interval<10, 40> / constrain_interval<1, 5>), (constrain_interval<2, 40>));
}
