#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe;
using namespace safe::literals;

TEST(safe_dsl_is_equal, simple_ivals) {
    EXPECT_EQ((ival<10, 20>), (ival<10, 20>));

    EXPECT_NE((ival<10, 20>), (ival<10, 10>));
}

TEST(safe_dsl_is_equal, union_ivals) {
    EXPECT_EQ((ival<10, 20> || ival<40, 50>), (ival<10, 20> || ival<40, 50>));

    EXPECT_EQ((ival<10, 20> || ival<40, 50>), (ival<40, 50> || ival<10, 20>));
}

TEST(safe_dsl_is_equal, differing_ival_types) {
    EXPECT_EQ((ival<10, 20>), (ival<10u, 20u>));

    EXPECT_EQ((ival<10ll, 20ll>), (ival<10, 20>));
}

TEST(safe_dsl_is_equal, with_delimeter) {
    EXPECT_EQ(1'2'3'4'5_u16, 12345_u16);
    EXPECT_EQ(0xc'0'ff'ee_u32, 0xc0ffee_u32);
}
