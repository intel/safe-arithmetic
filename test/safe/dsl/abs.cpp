#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_abs, ival_identity) {
    EXPECT_EQ(abs(constrain_interval<10, 20>), (constrain_interval<10, 20>));
}

TEST(safe_dsl_abs, ival_all_negative) {
    EXPECT_EQ(abs(constrain_interval<-30, -10>), (constrain_interval<10, 30>));
}

TEST(safe_dsl_abs, ival_straddle_min) {
    EXPECT_EQ(abs(constrain_interval<-30, 10>), (constrain_interval<0, 30>));
}

TEST(safe_dsl_abs, ival_straddle_max) {
    EXPECT_EQ(abs(constrain_interval<-30, 40>), (constrain_interval<0, 40>));
}

TEST(safe_dsl_abs, ival_straddle_eq) {
    EXPECT_EQ(abs(constrain_interval<-50, 50>), (constrain_interval<0, 50>));
}
