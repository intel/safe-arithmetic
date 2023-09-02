#include "properties.hpp"
#include "storage_gen.hpp"

#include <safe/big_integer/detail/multiplies.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace safe::_big_integer::detail {
RC_GTEST_PROP(multiplies, is_commutative, (storage<256> a, storage<256> b)) {
    is_commutative(multiplies, a, b);
}

RC_GTEST_PROP(multiplies, asymmetric_is_commutative,
              (storage<256> a, storage<128> b)) {
    is_commutative(multiplies, a, b);
}

RC_GTEST_PROP(multiplies, identity, (storage<128> a)) {
    identity<128, make_storage(1)>(multiplies, a);
}

RC_GTEST_PROP(multiplies, is_associative_32,
              (storage<32> a, storage<32> b, storage<32> c)) {
    is_associative<storage<96>>(multiplies, a, b, c);
}

RC_GTEST_PROP(multiplies, is_associative_64,
              (storage<64> a, storage<64> b, storage<64> c)) {
    is_associative<storage<192>>(multiplies, a, b, c);
}

RC_GTEST_PROP(multiplies, is_associative_128,
              (storage<128> a, storage<128> b, storage<128> c)) {
    is_associative<storage<384>>(multiplies, a, b, c);
}

RC_GTEST_PROP(multiplies, domination, (storage<128> a)) {
    domination<128, make_storage(0)>(multiplies, a);
}

RC_GTEST_PROP(multiplies, is_multiply_64, (int32_t a, int32_t b)) {
    auto a64 = static_cast<std::int64_t>(a);
    auto b64 = static_cast<std::int64_t>(b);
    storage<64> actual{};
    multiplies(actual, make_storage(a64), make_storage(b64));
    storage<64> expected = make_storage(a64 * b64);
    RC_ASSERT(actual == expected);
}

RC_GTEST_PROP(multiplies, is_multiply_32, (int32_t a, int32_t b)) {
    storage<64> actual{};
    multiplies(actual, make_storage(a), make_storage(b));
    storage<64> expected =
        make_storage(static_cast<int64_t>(a) * static_cast<int64_t>(b));
    RC_ASSERT(actual == expected);
}

RC_GTEST_PROP(multiplies, shift_and_multiply_equivalence, (storage<512> a)) {
    auto const i = *rc::gen::inRange<int32_t>(0, 511);

    storage<1024> shift_result{};
    bit_shift_left(shift_result, a, i);

    storage<1024> shifted_one{};
    bit_shift_left(shifted_one, make_storage(1), i);
    storage<1024> mul_result{};
    multiplies(mul_result, shifted_one, a);
    RC_ASSERT(shift_result == mul_result);
}
} // namespace safe::_big_integer::detail
