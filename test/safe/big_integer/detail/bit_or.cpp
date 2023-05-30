#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "storage_gen.hpp"
#include "properties.hpp"

#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/bitwise.hpp>


namespace safe::_big_integer::detail {
    RC_GTEST_PROP(bit_or, is_commutative, (storage<128> a, storage<128> b)) {
        is_commutative(bit_or, a, b);
    }

    RC_GTEST_PROP(bit_or, asymmetric_is_commutative, (storage<128> a, storage<64> b)) {
        is_commutative(bit_or, a, b);
    }

    RC_GTEST_PROP(bit_or, is_idempotent, (storage<128> a)) {
        is_idempotent(bit_or, a);
    }

    RC_GTEST_PROP(bit_or, identity, (storage<128> a)) {
        identity<128, storage<32>{}>(bit_or, a);
    }

    RC_GTEST_PROP(bit_or, domination, (storage<128> a)) {
        domination<128, storage<32>{{0xffff'ffffu}}>(bit_or, a);
    }

    RC_GTEST_PROP(bit_or, is_associative, (storage<128> a, storage<128> b, storage<128> c)) {
        is_associative<storage<128>>(bit_or, a, b, c);
    }

    RC_GTEST_PROP(bit_or, is_or, (int64_t a, int64_t b)) {
        storage<64> actual{}; bit_or(actual, make_storage(a), make_storage(b));
        storage<64> expected = make_storage(a | b);
        RC_ASSERT(actual == expected);
    }

    RC_GTEST_PROP(bit_or, lhs_can_be_result, (storage<256> a, storage<256> b)) {
        lhs_can_be_result<storage<512>>(bit_or, a, b);
    }

    TEST(bit_or, basic_opperation) {
        storage<128> lhs     {{0x0000'cafe, 0x1030'5070, 0x0550'0000, 0x0000'5555}};
        storage<128> rhs     {{0xffff'0000, 0x0204'0608, 0x7007'0000, 0x1111'0000}};
        storage<128> expected{{0xffff'cafe, 0x1234'5678, 0x7557'0000, 0x1111'5555}};
        bit_or(lhs, lhs, rhs);
        ASSERT_EQ(expected, lhs);
    }
}