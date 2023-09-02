#include "properties.hpp"
#include "storage_gen.hpp"

#include <safe/big_integer/detail/plus.hpp>
#include <safe/big_integer/detail/shift.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace safe::_big_integer::detail {
RC_GTEST_PROP(shift, identity_shift_left, (storage<128> a)) {
    identity<128, 0>(bit_shift_left, a);
}

RC_GTEST_PROP(shift, identity_shift_right, (storage<128> a)) {
    identity<128, 0>(bit_shift_right, a);
}

RC_GTEST_PROP(shift, shift_left_by_one_same_as_plus_itself, (storage<512> a)) {
    storage<512> shift_result{};
    bit_shift_left(shift_result, a, 1);
    storage<512> double_result{};
    plus(double_result, a, a);
    RC_ASSERT(shift_result == double_result);
}

RC_GTEST_PROP(shift, is_shift_left_64, (int64_t a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 63);
    storage<64> actual{};
    bit_shift_left(actual, make_storage(a), i);
    storage<64> expected = make_storage(a << i);
    RC_ASSERT(actual == expected);
}

RC_GTEST_PROP(shift, is_shift_left_32, (int32_t a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 31);
    storage<32> actual{};
    bit_shift_left(actual, make_storage(a), i);
    storage<32> expected = make_storage(a << i);
    RC_ASSERT(actual == expected);
}

RC_GTEST_PROP(shift, shift_left_order_doesnt_matter, (storage<512> a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 255);
    auto const j = *rc::gen::inRange<int32_t>(0, 255);
    storage<512> x0{};
    bit_shift_left(x0, a, i);
    storage<512> x1{};
    bit_shift_left(x1, x0, j);
    storage<512> y0{};
    bit_shift_left(y0, a, j);
    storage<512> y1{};
    bit_shift_left(y1, y0, i);
    RC_ASSERT(x1 == y1);
}

RC_GTEST_PROP(shift, shift_right_order_doesnt_matter, (storage<512> a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 255);
    auto const j = *rc::gen::inRange<int32_t>(0, 255);
    storage<512> x0{};
    bit_shift_right(x0, a, i);
    storage<512> x1{};
    bit_shift_right(x1, x0, j);
    storage<512> y0{};
    bit_shift_right(y0, a, j);
    storage<512> y1{};
    bit_shift_right(y1, y0, i);
    RC_ASSERT(x1 == y1);
}

RC_GTEST_PROP(shift, lhs_can_be_result_shift_left, (storage<256> a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 255);
    lhs_can_be_result<storage<512>>(bit_shift_left, a, i);
}

RC_GTEST_PROP(shift, lhs_can_be_result_shift_right, (storage<256> a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 255);
    lhs_can_be_result<storage<512>>(bit_shift_right, a, i);
}

RC_GTEST_PROP(shift, shift_left_then_right_is_same, (storage<256> a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 255);
    storage<512> r0{};
    bit_shift_left(r0, a, i);
    storage<512> r1{};
    bit_shift_right(r1, r0, i);
    RC_ASSERT(a == r1);
}

RC_GTEST_PROP(shift, is_shift_right_64, (int64_t a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 63);
    storage<64> actual{};
    bit_shift_right(actual, make_storage(a), i);
    storage<64> expected = make_storage(a >> i);
    RC_ASSERT(actual == expected);
}

RC_GTEST_PROP(shift, is_shift_right_32, (int32_t a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 31);
    storage<32> actual{};
    bit_shift_right(actual, make_storage(a), i);
    storage<32> expected = make_storage(a >> i);
    RC_ASSERT(actual == expected);
}
} // namespace safe::_big_integer::detail