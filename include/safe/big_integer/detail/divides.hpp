#pragma once

#include <safe/big_integer/detail/algorithms.hpp>
#include <safe/big_integer/detail/compare.hpp>
#include <safe/big_integer/detail/plus.hpp>
#include <safe/big_integer/detail/shift.hpp>
#include <safe/big_integer/detail/storage.hpp>

#include <concepts>
#include <functional>

namespace safe::_big_integer::detail {
template <std::size_t LhsNumBits, std::size_t RhsNumBits>
[[nodiscard]] constexpr auto largest_doubling(storage<LhsNumBits> a,
                                              storage<RhsNumBits> b) {
    using ret_t = storage<std::max(LhsNumBits, RhsNumBits) + 1>;

    auto ret = ret_t{b};

    auto const not_big_enough = [&]() -> bool {
        auto a_prime = a;
        minus(a_prime, a_prime, ret);
        return ret <= a_prime;
    };

    while (not_big_enough()) {
        bit_shift_left(ret, ret, 1);
    }

    return ret;
}

constexpr static auto unsigned_divmod = [](auto &quotient, auto &remainder,
                                           auto const &dividend,
                                           auto const &divisor) -> void {
    // NOTE: Algorithm is based on "Elements of Programming"
    // section 5.7 Quotient

    auto a = dividend;
    auto b = divisor;

    if (a < b) {
        quotient = make_storage(0);
        remainder = a;

    } else {
        constexpr auto one = make_storage(1);

        auto c = largest_doubling(a, b);
        minus(a, a, c);
        quotient = one;
        while (c != b) {
            bit_shift_left(quotient, quotient, 1);
            bit_shift_right(c, c, 1);
            if (c <= a) {
                minus(a, a, c);
                plus(quotient, quotient, one);
            }
        }

        remainder = a;
    }
};

constexpr static auto divmod = [](auto &quotient, auto &remainder,
                                  auto const &dividend,
                                  auto const &divisor) -> void {
    auto a = dividend;
    auto b = divisor;

    if (a.negative()) {
        if (b.negative()) {
            negate(a, a);
            negate(b, b);
            unsigned_divmod(quotient, remainder, a, b);

        } else {
            negate(a, a);
            unsigned_divmod(quotient, remainder, a, b);
            negate(quotient, quotient);
        }

        negate(remainder, remainder);

    } else {
        if (b.negative()) {
            negate(b, b);
            unsigned_divmod(quotient, remainder, a, b);
            negate(quotient, quotient);

        } else {
            unsigned_divmod(quotient, remainder, a, b);
        }
    }
};
} // namespace safe::_big_integer::detail
