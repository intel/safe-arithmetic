#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include <safe.hpp>

using ::testing::_;
using ::testing::InSequence;
using ::testing::Return;

using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;

using safe::constrain_interval;
using safe::constrain_mask;

template <typename T> class safe_int_assign_test : public testing::Test {};

using safe_int_types =
    ::testing::Types<safe::u8, safe::u16, safe::u32, safe::u64, safe::s8,
                     safe::s16, safe::s32, safe::s64>;

TYPED_TEST_SUITE(safe_int_assign_test, safe_int_types);

TYPED_TEST(safe_int_assign_test, init_vars) {
    TypeParam value{42_cn};
    EXPECT_EQ(value.unsafe_value(), 42);
}

TYPED_TEST(safe_int_assign_test, assign_vars) {
    TypeParam value{0_cn};
    value = 33_cn;
    EXPECT_EQ(value.unsafe_value(), 33);
}

template <typename T> class safe_var_ops_test : public testing::Test {};

using std_int_types = ::testing::Types<uint8_t, uint16_t, uint32_t, uint64_t,
                                       int8_t, int16_t, int32_t, int64_t>;

TYPED_TEST_SUITE(safe_var_ops_test, std_int_types);

TYPED_TEST(safe_var_ops_test, add_op) {
    safe::constrained_number<constrain_interval<0, 50>, TypeParam> const a = 13_cn;
    safe::constrained_number<constrain_interval<0, 50>, TypeParam> const b = 29_cn;
    auto const result = a + b;
    EXPECT_EQ(result.unsafe_value(), 42);
}

TYPED_TEST(safe_var_ops_test, minus_op) {
    safe::constrained_number<constrain_interval<30, 100>, TypeParam> const a = 32_cn;
    safe::constrained_number<constrain_interval<0, 30>, TypeParam> const b = 8_cn;
    auto const result = a - b;
    EXPECT_EQ(result.unsafe_value(), 24);
}

TYPED_TEST(safe_var_ops_test, multiply_op) {
    safe::constrained_number<constrain_interval<0, 10>, TypeParam> const a = 6_cn;
    safe::constrained_number<constrain_interval<0, 10>, TypeParam> const b = 8_cn;
    auto const result = a * b;
    EXPECT_EQ(result.unsafe_value(), 48);
}

TYPED_TEST(safe_var_ops_test, divide_op) {
    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const a = 45_cn;
    safe::constrained_number<constrain_interval<1, 100>, TypeParam> const b = 5_cn;
    auto const result = a / b;
    EXPECT_EQ(result.unsafe_value(), 9);
}

// FIXME: MODULO NEEDS TO BE REWRITTEN
// TYPED_TEST(safe_var_ops_test, modulo_op) {
//    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const a = 12_cn;
//    safe::constrained_number<constrain_interval<1, 100>, TypeParam> const b = 5_cn;
//    auto const result = a % b;
//    EXPECT_EQ(result.unsafe_value(), 2);
//}

// TYPED_TEST(safe_var_ops_test, left_shift_op) {
//    safe::constrained_number<constrain_interval<0, 8>, TypeParam> const a = 8_cn;
//    safe::constrained_number<constrain_interval<0, 3>, TypeParam> const b = 2_cn;
//    auto const result = a << b;
//    EXPECT_EQ(result.unsafe_value(), 32);
//}

// TYPED_TEST(safe_var_ops_test, right_shift_op) {
//    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const a = 48_cn;
//    safe::constrained_number<constrain_interval<0, 8>, TypeParam> const b = 2_cn;
//    auto const result = a >> b;
//    EXPECT_EQ(result.unsafe_value(), 12);
//}

TYPED_TEST(safe_var_ops_test, spaceship_op) {
    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const a = 45_cn;
    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const b = 87_cn;

    EXPECT_TRUE(a == a);
    EXPECT_TRUE(b == b);
    EXPECT_TRUE(b >= b);
    EXPECT_TRUE(b <= b);

    EXPECT_FALSE(a == b);
    EXPECT_TRUE(a != b);

    EXPECT_TRUE(a < b);
    EXPECT_TRUE(b > a);
    EXPECT_TRUE(a <= b);
    EXPECT_TRUE(b >= a);

    EXPECT_FALSE(a > b);
    EXPECT_FALSE(b < a);
    EXPECT_FALSE(a >= b);
    EXPECT_FALSE(b <= a);
}

TYPED_TEST(safe_var_ops_test, max_op) {
    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const a = 48_cn;
    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const b = 32_cn;
    auto const result = std::max(a, b);
    EXPECT_EQ(result.unsafe_value(), 48);
}

TYPED_TEST(safe_var_ops_test, min_op) {
    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const a = 48_cn;
    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const b = 32_cn;
    auto const result = std::min(a, b);
    EXPECT_EQ(result.unsafe_value(), 32);
}

TYPED_TEST(safe_var_ops_test, clamp_op) {
    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const min = 32_cn;
    safe::constrained_number<constrain_interval<0, 100>, TypeParam> const max = 48_cn;
    auto const result = clamp(65, min, max);
    EXPECT_EQ(result.unsafe_value(), 48);
}

TEST(safe_var_test, init_primitive_1) {
    safe::s64 result{-42};
    EXPECT_EQ(result.unsafe_value(), -42);
}

TEST(safe_var_test, init_primitive_2) {
    safe::s64 result = -42;
    EXPECT_EQ(result.unsafe_value(), -42);
}

TEST(safe_var_test, assign_primitive) {
    safe::s64 result{};
    result = -42;
    EXPECT_EQ(result.unsafe_value(), -42);
}

TEST(safe_var_test, init_int_const_1) {
    safe::s64 result{std::integral_constant<uint32_t, 89>{}};
    EXPECT_EQ(result.unsafe_value(), 89);
}

TEST(safe_var_test, init_int_const_2) {
    safe::s64 result = std::integral_constant<uint32_t, 89>{};
    EXPECT_EQ(result.unsafe_value(), 89);
}

TEST(safe_var_test, assign_int_const) {
    safe::s64 result{};
    result = std::integral_constant<int32_t, -55>{};
    EXPECT_EQ(result.unsafe_value(), -55);
}

TEST(safe_var_test, negate_op) {
    auto const result = -42_cn;
    EXPECT_EQ(result.unsafe_value(), -42);
}

TEST(safe_var_test, abs_op) {
    auto const result = abs(-42_cn);
    EXPECT_EQ(result.unsafe_value(), 42);
}

TEST(safe_var_test, bitwise_or_op) {
    safe::constrained_number<constrain_mask<0xff>, uint32_t> const a = 15_cn;
    safe::constrained_number<constrain_mask<0xff>, uint32_t> const b = 9_cn;
    auto const result = a & b;
    EXPECT_EQ(result.unsafe_value(), 9);
}

TEST(safe_var_test, add_int_const) {
    safe::constrained_number<constrain_interval<0, 100>, uint32_t> const a = 15_cn;
    auto const result = a + std::integral_constant<int, 10>{};
    EXPECT_EQ(result.unsafe_value(), 25);
}

// FIXME: need to automatically convert integral_constants to safe::constrained_number
TEST(safe_var_test, use_case_bitfield_extract_1) {
    safe::u32 const reg = u32_<0xba5eba11>;
    auto const field = (reg >> u32_<16>)&u32_<0xff>;

    EXPECT_TRUE(field.constraint <= constrain_mask<0xff>);
    EXPECT_EQ(field.unsafe_value(), 0x5e);
}

TEST(safe_var_test, use_case_bitfield_extract_2) {
    auto const reg = u32_<0xba5eba11>;
    auto const field = (reg >> u32_<16>)&u32_<0xff>;

    EXPECT_TRUE(field.constraint <= constrain_mask<0xff>);
    EXPECT_EQ(field.unsafe_value(), 0x5e);
}
