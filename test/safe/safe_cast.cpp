#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <cstdint>
#include <type_traits>

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;

TEST(safe_cast_test, cast_same_type) {
    auto v = safe_cast<std::int32_t>(42_s32);
    EXPECT_EQ(v, 42);
    static_assert(std::is_same_v<std::int32_t, decltype(v)>);
}

TEST(safe_cast_test, cast_narrower_type) {
    auto v = safe_cast<std::uint8_t>(42_s32);
    EXPECT_EQ(v, 42);
    static_assert(std::is_same_v<std::uint8_t, decltype(v)>);
}

TEST(safe_cast_test, cast_different_sign) {
    auto v = safe_cast<std::uint32_t>(99_s32);
    EXPECT_EQ(v, 99);
    static_assert(std::is_same_v<std::uint32_t, decltype(v)>);
}
