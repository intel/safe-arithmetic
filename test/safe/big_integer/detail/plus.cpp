#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "storage_gen.hpp"
#include "properties.hpp"

#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/plus.hpp>


namespace safe::_big_integer::detail {
    RC_GTEST_PROP(plus, is_commutative, (storage<128> a, storage<128> b)) {
        is_commutative(plus, a, b);
    }

    RC_GTEST_PROP(plus, asymmetric_is_commutative, (storage<256> a, storage<128> b)) {
        is_commutative(plus, a, b);
    }

    RC_GTEST_PROP(plus, identity, (storage<128> a)) {
        identity<128, storage<32>{}>(plus, a);
    }

    RC_GTEST_PROP(plus, is_associative, (storage<128> a, storage<128> b, storage<128> c)) {
        is_associative<storage<130>>(plus, a, b, c);
    }

    RC_GTEST_PROP(plus, negative_self_is_domination, (storage<128> a)) {
        storage<128> neg_a{}; negate(neg_a, a);
        storage<128> actual{}; plus(actual, a, neg_a);
        storage<128> zero{};
        RC_ASSERT(actual == zero);
    }

    RC_GTEST_PROP(plus, is_plus, (int64_t a, int64_t b)) {
        storage<64> actual{}; plus(actual, make_storage(a), make_storage(b));
        storage<64> expected = make_storage(a + b);
        RC_ASSERT(actual == expected);
    }

    RC_GTEST_PROP(plus, lhs_can_be_result, (storage<256> a, storage<256> b)) {
        lhs_can_be_result<storage<512>>(plus, a, b);
    }
}