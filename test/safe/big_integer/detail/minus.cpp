#include "properties.hpp"
#include "storage_gen.hpp"

#include <safe/big_integer/detail/plus.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace safe::_big_integer::detail {
RC_GTEST_PROP(minus, identity, (storage<128> a)) {
    identity<128, storage<32>{}>(minus, a);
}

RC_GTEST_PROP(minus, self_is_domination, (storage<128> a)) {
    storage<128> actual{};
    minus(actual, a, a);
    storage<128> zero{};
    RC_ASSERT(actual == zero);
}

RC_GTEST_PROP(minus, is_minus, (int64_t a, int64_t b)) {
    RC_PRE(subtraction_will_not_overflow(a, b));
    storage<64> actual{};
    minus(actual, make_storage(a), make_storage(b));
    storage<64> expected = make_storage(a - b);
    RC_ASSERT(actual == expected);
}
} // namespace safe::_big_integer::detail
