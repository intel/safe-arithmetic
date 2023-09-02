#include "properties.hpp"
#include "storage_gen.hpp"

#include <safe/big_integer/detail/bitwise.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace safe::_big_integer::detail {
RC_GTEST_PROP(bit_not, not_not_is_self, (storage<128> a)) {
    storage<128> r{};
    bit_not(r, a);
    bit_not(r, r);
    RC_ASSERT(r == a);
}

RC_GTEST_PROP(bit_not, is_not, (int64_t a)) {
    storage<64> actual{};
    bit_not(actual, make_storage(a));
    storage<64> expected = make_storage(~a);
    RC_ASSERT(actual == expected);
}
} // namespace safe::_big_integer::detail