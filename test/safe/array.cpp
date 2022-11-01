#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;


using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;


TEST(safe_array_test, construction) {
    safe::array<int, 16> t{};
}

TEST(safe_array_test, bracket_op) {
    safe::array<int, 16> t{};

    t[12_s32] = 42;

    EXPECT_EQ(t[12_s32], 42);
}

TEST(safe_array_test, const_bracket_op) {
    safe::array const t{0, 13, 2};

    EXPECT_EQ(t[1_s32], 13);
}

TEST(safe_array_test, at) {
    safe::array<int, 16> t{};

    t[12_s32] = 42;

    EXPECT_EQ(t.at(12_s32), 42);
}

TEST(safe_array_test, const_at) {
    safe::array const t{0, 13, 2};

    EXPECT_EQ(t.at(1_s32), 13);
}
