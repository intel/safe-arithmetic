#pragma once

#include <safe/big_integer.hpp>

#include <algorithm>
#include <cstdint>
#include <limits>
#include <type_traits>

namespace safe::dsl {
template <typename T> struct triint {
  private:
    T var_bits_;
    T const_bits_;

  public:
    constexpr triint() : var_bits_{}, const_bits_{} {}

    constexpr triint(T var_bits, T const_bits)
        : var_bits_{var_bits}, const_bits_{const_bits & ~var_bits} {}

    [[nodiscard]] constexpr T var_bits() const { return var_bits_; }

    [[nodiscard]] constexpr T const_bits() const { return const_bits_; }
};

template <typename LhsT, typename RhsT>
[[nodiscard]] constexpr auto operator==(triint<LhsT> lhs, triint<RhsT> rhs)
    -> bool {
    return (lhs.var_bits() == rhs.var_bits()) &&
           (lhs.const_bits() == rhs.const_bits());
}

template <typename LhsT, typename RhsT>
[[nodiscard]] constexpr auto operator&(triint<LhsT> lhs, triint<RhsT> rhs)
    -> triint<common_integer_t<LhsT, RhsT>> {
    return {((lhs.var_bits() & (rhs.const_bits() | rhs.var_bits())) |
             (rhs.var_bits() & (lhs.const_bits() | lhs.var_bits()))),
            lhs.const_bits() & rhs.const_bits()};
}

template <typename LhsT, typename RhsT>
[[nodiscard]] constexpr auto operator|(triint<LhsT> lhs, triint<RhsT> rhs)
    -> triint<common_integer_t<LhsT, RhsT>> {
    auto const lhs_const_bits = lhs.const_bits() & ~lhs.var_bits();
    auto const rhs_const_bits = rhs.const_bits() & ~rhs.var_bits();

    return {((lhs.var_bits() & ~rhs_const_bits) |
             (rhs.var_bits() & ~lhs_const_bits)),
            lhs_const_bits | rhs_const_bits};
}

template <typename LhsT, typename RhsT>
[[nodiscard]] constexpr auto operator^(triint<LhsT> lhs, triint<RhsT> rhs)
    -> triint<common_integer_t<LhsT, RhsT>> {
    return {lhs.var_bits() | rhs.var_bits(),
            lhs.const_bits() ^ rhs.const_bits()};
}

template <typename U>
[[nodiscard]] constexpr auto operator~(triint<U> val) -> triint<U> {
    return {val.var_bits(), ~val.const_bits()};
}

template <typename U>
[[nodiscard]] constexpr auto operator<<(triint<U> val, auto shamt)
    -> triint<U> {
    return {val.var_bits() << shamt, val.const_bits() << shamt};
}

template <typename U>
[[nodiscard]] constexpr auto operator>>(triint<U> val, auto shamt)
    -> triint<U> {
    return {val.var_bits() >> shamt, val.const_bits() >> shamt};
}

template <typename LhsT, typename RhsT, typename CarryInT, typename CarryOutT>
[[nodiscard]] constexpr auto full_adder(triint<LhsT> lhs, triint<RhsT> rhs,
                                        triint<CarryInT> carry_in,
                                        triint<CarryOutT> &carry_out) {
    carry_out = (lhs & rhs) | (carry_in & (lhs ^ rhs));
    return lhs ^ rhs ^ carry_in;
}

template <typename LhsT, typename RhsT>
[[nodiscard]] constexpr auto operator+(triint<LhsT> lhs, triint<RhsT> rhs)
    -> triint<common_integer_t<LhsT, RhsT>> {
    using int_t = common_integer_t<LhsT, RhsT>;
    triint<int_t> result{};
    triint<int_t> carry{};

    constexpr auto num_bits =
        std::numeric_limits<int_t>::digits + (std::is_signed_v<int_t> ? 1 : 0);

    for (int i = 0; i < num_bits; i++) {
        auto const digit = triint<int_t>{0, static_cast<int_t>(1 << i)};
        auto const a = lhs & digit;
        auto const b = rhs & digit;
        auto const sum = full_adder(a, b, carry, carry);
        carry = carry << 1;
        result = result | sum;
    }

    return result;
}

template <typename LhsT, typename RhsT>
[[nodiscard]] constexpr auto operator-(triint<LhsT> lhs, triint<RhsT> rhs)
    -> triint<common_integer_t<LhsT, RhsT>> {
    using int_t = common_integer_t<LhsT, RhsT>;
    return lhs + ~rhs + triint{int_t{0}, int_t{1}};
}

template <typename T, typename U>
triint(T var_bits, U const_bits) -> triint<common_integer_t<T, U>>;
} // namespace safe::dsl