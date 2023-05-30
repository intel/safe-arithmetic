#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "storage_gen.hpp"
#include "properties.hpp"

#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/bitwise.hpp>


namespace safe::_big_integer::detail {
    RC_GTEST_PROP(bit_and, is_commutative, (storage<128> a, storage<128> b)) {
        is_commutative(bit_and, a, b);
    }

    RC_GTEST_PROP(bit_and, asymmetric_is_commutative, (storage<128> a, storage<64> b)) {
        is_commutative(bit_and, a, b);
    }

    RC_GTEST_PROP(bit_and, is_idempotent, (storage<128> a)) {
        is_idempotent(bit_and, a);
    }

    RC_GTEST_PROP(bit_and, identity, (storage<128> a)) {
        identity<128, storage<32>{{0xffff'ffffu}}>(bit_and, a);
    }

    RC_GTEST_PROP(bit_and, domination, (storage<128> a)) {
        domination<128, storage<32>{}>(bit_and, a);
    }

    RC_GTEST_PROP(bit_and, is_associative, (storage<128> a, storage<128> b, storage<128> c)) {
        is_associative<storage<128>>(bit_and, a, b, c);
    }

    RC_GTEST_PROP(bit_and, is_and, (int64_t a, int64_t b)) {
        storage<64> actual{}; bit_and(actual, make_storage(a), make_storage(b));
        storage<64> expected = make_storage(a & b);
        RC_ASSERT(actual == expected);
    }

    RC_GTEST_PROP(bit_and, lhs_can_be_result, (storage<256> a, storage<256> b)) {
        lhs_can_be_result<storage<512>>(bit_and, a, b);
    }

    TEST(bit_and, basic_operation) {
        storage<128> lhs     {{0x0000'ffff, 0x1030'ffff, 0x0550'ffff, 0x0000'5555}};
        storage<128> rhs     {{0xffff'0000, 0xffff'0608, 0xffff'0000, 0xffff'0000}};
        storage<128> expected{{0x0000'0000, 0x1030'0608, 0x0550'0000, 0x0000'0000}};
        bit_and(lhs, lhs, rhs);
        ASSERT_EQ(expected, lhs);
    }
}