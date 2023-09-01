#include "properties.hpp"
#include "storage_gen.hpp"

#include <safe/big_integer/detail/divides.hpp>
#include <safe/big_integer/detail/multiplies.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <gtest/gtest.h>
#include <rapidcheck/gtest.h>

namespace safe::_big_integer::detail {
RC_GTEST_PROP(unsigned_divmod, identity, (storage<128> dividend)) {
    RC_PRE(!dividend.negative());
    storage<128> q{};
    storage<128> r{};
    unsigned_divmod(q, r, dividend, make_storage(1));
    RC_ASSERT(q == dividend);
    RC_ASSERT(r == make_storage(0));
}

RC_GTEST_PROP(unsigned_divmod, dividend_is_zero, (storage<128> divisor)) {
    RC_PRE(!divisor.negative());
    RC_PRE(divisor != make_storage(0));
    storage<128> q{};
    storage<128> r{};
    unsigned_divmod(q, r, make_storage(0), divisor);
    RC_ASSERT(q == make_storage(0));
    RC_ASSERT(r == make_storage(0));
}

RC_GTEST_PROP(unsigned_divmod, divide_then_multiply,
              (storage<128> dividend, storage<128> divisor)) {
    RC_PRE(!divisor.negative());
    RC_PRE(!dividend.negative());
    RC_PRE(divisor != make_storage(0));

    storage<128> q{};
    storage<128> r{};

    unsigned_divmod(q, r, dividend, divisor);

    storage<128> dividend_prime{};
    multiplies(dividend_prime, q, divisor);
    plus(dividend_prime, dividend_prime, r);

    RC_ASSERT(dividend_prime == dividend);
}

RC_GTEST_PROP(divmod, identity, (storage<128> dividend)) {
    storage<128> q{};
    storage<128> r{};
    divmod(q, r, dividend, make_storage(1));
    RC_ASSERT(q == dividend);
    RC_ASSERT(r == make_storage(0));
}

RC_GTEST_PROP(divmod, dividend_is_zero, (storage<128> divisor)) {
    RC_PRE(divisor != make_storage(0));
    storage<128> q{};
    storage<128> r{};
    divmod(q, r, make_storage(0), divisor);
    RC_ASSERT(q == make_storage(0));
    RC_ASSERT(r == make_storage(0));
}

RC_GTEST_PROP(divmod, divide_then_multiply,
              (storage<256> dividend, storage<256> divisor)) {
    RC_PRE(divisor != make_storage(0));

    storage<256> q{};
    storage<256> r{};

    divmod(q, r, dividend, divisor);

    storage<256> dividend_prime{};
    multiplies(dividend_prime, q, divisor);
    plus(dividend_prime, dividend_prime, r);

    RC_ASSERT(dividend_prime == dividend);
}

RC_GTEST_PROP(divmod, is_divide_and_remainder_64,
              (int64_t dividend, int64_t divisor)) {
    RC_PRE(divisor != 0);

    storage<64> q{};
    storage<64> r{};

    divmod(q, r, make_storage(dividend), make_storage(divisor));

    RC_ASSERT(make_storage(dividend / divisor) == q);
    RC_ASSERT(make_storage(dividend % divisor) == r);
}

RC_GTEST_PROP(divmod, is_divide_and_remainder_32,
              (int32_t dividend, int32_t divisor)) {
    RC_PRE(divisor != 0);

    storage<32> q{};
    storage<32> r{};

    divmod(q, r, make_storage(dividend), make_storage(divisor));

    RC_ASSERT(make_storage(dividend / divisor) == q);
    RC_ASSERT(make_storage(dividend % divisor) == r);
}
} // namespace safe::_big_integer::detail