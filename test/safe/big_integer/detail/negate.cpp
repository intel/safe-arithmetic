#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

#include "storage_gen.hpp"
#include "properties.hpp"

#include <safe/big_integer/detail/storage.hpp>
#include <safe/big_integer/detail/plus.hpp>


namespace safe::_big_integer::detail {
    RC_GTEST_PROP(negate, negate_negate_is_self, (storage<128> a)) {
        storage<128> r{}; negate(r, a); negate(r, r);
        RC_ASSERT(r == a);
    }

    RC_GTEST_PROP(negate, is_negate, (int64_t a)) {
        storage<64> actual{}; negate(actual, make_storage(a));
        storage<64> expected = make_storage(-a);
        RC_ASSERT(actual == expected);
    }
}