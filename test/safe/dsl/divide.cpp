#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_divide, divide_two_ival_constants) {
    EXPECT_EQ((ival<48, 48> / ival<4, 4>), (ival<12, 12>));
}

TEST(safe_dsl_divide, add_two_intervals) {
    EXPECT_EQ((ival<10, 40> / ival<1, 5>), (ival<2, 40>));
}
