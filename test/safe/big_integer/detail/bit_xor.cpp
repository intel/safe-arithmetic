#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "storage_gen.hpp"
#include "properties.hpp"

#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/bitwise.hpp>


namespace safe::_big_integer::detail {
    RC_GTEST_PROP(bit_xor, is_commutative, (storage<128> a, storage<128> b)) {
        is_commutative(bit_xor, a, b);
    }

    RC_GTEST_PROP(bit_xor, asymmetric_is_commutative, (storage<128> a, storage<64> b)) {
        is_commutative(bit_xor, a, b);
    }

    RC_GTEST_PROP(bit_xor, is_associative, (storage<128> a, storage<128> b, storage<128> c)) {
        is_associative<storage<128>>(bit_xor, a, b, c);
    }

    RC_GTEST_PROP(bit_xor, identity, (storage<128> a)) {
        identity<128, storage<32>{}>(bit_xor, a);
    }

    RC_GTEST_PROP(bit_xor, self_is_domination, (storage<128> a)) {
        storage<128> r{}; bit_xor(r, a, a);
        storage<128> zero{};

        RC_ASSERT(r == zero);
    }

    RC_GTEST_PROP(bit_xor, is_xor, (int64_t a, int64_t b)) {
        storage<64> actual{}; bit_xor(actual, make_storage(a), make_storage(b));
        storage<64> expected = make_storage(a ^ b);
        RC_ASSERT(actual == expected);
    }

    RC_GTEST_PROP(bit_xor, lhs_can_be_result, (storage<256> a, storage<256> b)) {
        lhs_can_be_result<storage<512>>(bit_xor, a, b);
    }

    TEST(bit_xor, basic_operation) {
        storage<128> lhs     {{0x0000'ffff, 0x1030'ffff, 0x0550'ffff, 0x0000'5555}};
        storage<128> rhs     {{0xffff'0000, 0xffff'0608, 0xffff'0000, 0xffff'0000}};
        storage<128> expected{{0xffff'ffff, 0xefcf'f9f7, 0xfaaf'ffff, 0xffff'5555}};
        bit_xor(lhs, lhs, rhs);
        ASSERT_EQ(expected, lhs);
    }
}