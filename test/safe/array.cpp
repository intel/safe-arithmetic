#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;

TEST(safe_array_test, construction) { safe::array<int, 16> t{}; }

TEST(safe_array_test, bracket_op) {
    safe::array<int, 16> t{};

    t[12_cn] = 42;

    EXPECT_EQ(t[12_cn], 42);
}

TEST(safe_array_test, const_bracket_op) {
    safe::array const t{0, 13, 2};

    EXPECT_EQ(t[1_cn], 13);
}

TEST(safe_array_test, at) {
    safe::array<int, 16> t{};

    t[12_cn] = 42;

    EXPECT_EQ(t.at(12_cn), 42);
}

TEST(safe_array_test, const_at) {
    safe::array const t{0, 13, 2};

    EXPECT_EQ(t.at(1_cn), 13);
}
