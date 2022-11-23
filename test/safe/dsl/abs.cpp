#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

using namespace safe;
using namespace safe::literals;


TEST(safe_dsl_abs, ival_identity) {
    EXPECT_EQ(
        abs(ival<10, 20>),
        (ival<10, 20>)
    );
}

TEST(safe_dsl_abs, ival_all_negative) {
    EXPECT_EQ(
        abs(ival<-30, -10>),
        (ival<10, 30>)
    );
}

TEST(safe_dsl_abs, ival_straddle_min) {
    EXPECT_EQ(
        abs(ival<-30, 10>),
        (ival<0, 30>)
    );
}

TEST(safe_dsl_abs, ival_straddle_max) {
    EXPECT_EQ(
        abs(ival<-30, 40>),
        (ival<0, 40>)
    );
}

TEST(safe_dsl_abs, ival_straddle_eq) {
    EXPECT_EQ(
        abs(ival<-50, 50>),
        (ival<0, 50>)
    );
}
