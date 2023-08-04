#include "gtest/gtest.h"
#include "gmock/gmock.h"

#include <safe.hpp>


using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;


using namespace safe::interval_types;
using namespace safe::int_types;
using namespace safe::literals;

using safe::ival;
using safe::mask;

template <typename T>
class safe_int_assign_test : public testing::Test { };

using safe_int_types = ::testing::Types<
    safe::u8, safe::u16, safe::u32, safe::u64,
    safe::s8, safe::s16, safe::s32, safe::s64>;

TYPED_TEST_SUITE(safe_int_assign_test, safe_int_types);

TYPED_TEST(safe_int_assign_test, init_vars) {
    TypeParam value{42_s32};
    EXPECT_EQ(value.unsafe_value(), 42);
}

TYPED_TEST(safe_int_assign_test, assign_vars) {
    TypeParam value{0_s32};
    value = 33_s32;
    EXPECT_EQ(value.unsafe_value(), 33);
}

template <typename T>
class safe_var_ops_test : public testing::Test { };

using std_int_types = ::testing::Types<
    uint8_t, uint16_t, uint32_t, uint64_t,
     int8_t,  int16_t,  int32_t,  int64_t>;

TYPED_TEST_SUITE(safe_var_ops_test, std_int_types);

TYPED_TEST(safe_var_ops_test, add_op) {
    safe::var<TypeParam, ival<0, 50>> const a = 13_s32;
    safe::var<TypeParam, ival<0, 50>> const b = 29_s32;
    auto const result = a + b;
    EXPECT_EQ(result.unsafe_value(), 42);
}

TYPED_TEST(safe_var_ops_test, minus_op) {
    safe::var<TypeParam, ival<30, 100>> const a = 32_s32;
    safe::var<TypeParam, ival<0, 30>> const b = 8_s32;
    auto const result = a - b;
    EXPECT_EQ(result.unsafe_value(), 24);
}

TYPED_TEST(safe_var_ops_test, multiply_op) {
    safe::var<TypeParam, ival<0, 10>> const a = 6_s32;
    safe::var<TypeParam, ival<0, 10>> const b = 8_s32;
    auto const result = a * b;
    EXPECT_EQ(result.unsafe_value(), 48);
}

TYPED_TEST(safe_var_ops_test, divide_op) {
    safe::var<TypeParam, ival<0, 100>> const a = 45_s32;
    safe::var<TypeParam, ival<1, 100>> const b = 5_s32;
    auto const result = a / b;
    EXPECT_EQ(result.unsafe_value(), 9);
}

// FIXME: MODULO NEEDS TO BE REWRITTEN
//TYPED_TEST(safe_var_ops_test, modulo_op) {
//    safe::var<TypeParam, ival<0, 100>> const a = 12_s32;
//    safe::var<TypeParam, ival<1, 100>> const b = 5_s32;
//    auto const result = a % b;
//    EXPECT_EQ(result.unsafe_value(), 2);
//}

// FIXME: big_integer needs variable left shift support
//TYPED_TEST(safe_var_ops_test, left_shift_op) {
//    safe::var<TypeParam, ival<0, 8>> const a = 8_s32;
//    safe::var<TypeParam, ival<0, 3>> const b = 2_s32;
//    auto const result = a << b;
//    EXPECT_EQ(result.unsafe_value(), 32);
//}

// FIXME: big_integer needs variable right shift support
//TYPED_TEST(safe_var_ops_test, right_shift_op) {
//    safe::var<TypeParam, ival<0, 100>> const a = 48_s32;
//    safe::var<TypeParam, ival<0, 8>> const b = 2_s32;
//    auto const result = a >> b;
//    EXPECT_EQ(result.unsafe_value(), 12);
//}

TYPED_TEST(safe_var_ops_test, spaceship_op) {
    safe::var<TypeParam, ival<0, 100>> const a = 45_s32;
    safe::var<TypeParam, ival<0, 100>> const b = 87_s32;

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
    safe::var<TypeParam, ival<0, 100>> const a = 48_s32;
    safe::var<TypeParam, ival<0, 100>> const b = 32_s32;
    auto const result = std::max(a, b);
    EXPECT_EQ(result.unsafe_value(), 48);
}

TYPED_TEST(safe_var_ops_test, min_op) {
    safe::var<TypeParam, ival<0, 100>> const a = 48_s32;
    safe::var<TypeParam, ival<0, 100>> const b = 32_s32;
    auto const result = std::min(a, b);
    EXPECT_EQ(result.unsafe_value(), 32);
}

TYPED_TEST(safe_var_ops_test, clamp_op) {
    safe::var<TypeParam, ival<0, 100>> const min = 32_s32;
    safe::var<TypeParam, ival<0, 100>> const max = 48_s32;
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
    auto const result = -42_s32;
    EXPECT_EQ(result.unsafe_value(), -42);
}

TEST(safe_var_test, abs_op) {
    auto const result = abs(-42_s32);
    EXPECT_EQ(result.unsafe_value(), 42);
}

TEST(safe_var_test, bitwise_or_op) {
    safe::var<uint32_t, mask<0xff>> const a = 15_s32;
    safe::var<uint32_t, mask<0xff>> const b = 9_s32;
    auto const result = a & b;
    EXPECT_EQ(result.unsafe_value(), 9);
}

TEST(safe_var_test, add_int_const) {
    safe::var<uint32_t, ival<0, 100>> const a = 15_s32;
    auto const result = a + std::integral_constant<int, 10>{};
    EXPECT_EQ(result.unsafe_value(), 25);
}

// FIXME: need to automatically convert integral_constants to safe::var
TEST(safe_var_test, use_case_bitfield_extract_1) {
   safe::u32 const reg = u32_<0xba5eba11>;
   auto const field = (reg >> u32_<16>) & u32_<0xff>;

   EXPECT_TRUE(field.requirement <= mask<0xff>);
   EXPECT_EQ(field.unsafe_value(), 0x5e);
}

TEST(safe_var_test, use_case_bitfield_extract_2) {
   auto const reg = u32_<0xba5eba11>;
   auto const field = (reg >> u32_<16>) & u32_<0xff>;

   EXPECT_TRUE(field.requirement <= mask<0xff>);
   EXPECT_EQ(field.unsafe_value(), 0x5e);
}

