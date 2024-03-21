#pragma once

#include <safe/big_integer.hpp>
#include <safe/detail/pure.hpp>
#include <safe/dsl/detail/triint.hpp>
#include <safe/dsl/fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>

namespace safe::dsl {
template <auto VariableBits, auto ConstantBits = 0>
struct constrain_mask_t : public detail::primitive {
    using type = constrain_mask_t;

    constexpr static auto var_bits = VariableBits;
    constexpr static auto const_bits = ConstantBits;
    constexpr static auto value = triint{var_bits, const_bits};

    [[nodiscard]] SAFE_PURE constexpr static auto check(auto value) -> bool {
        return (~var_bits & value) == (~var_bits & const_bits);
    }
};

template <auto VariableBits, auto ConstantBits = 0>
constexpr constrain_mask_t<to_big_integer(VariableBits), to_big_integer(ConstantBits)>
    constrain_mask{};

namespace detail {
/**
 * Takes a bitmask and sets all digits to the right of the first
 * '1' to '1'.
 */
template <typename T> [[nodiscard]] constexpr auto fill_in_bitmask(T value) {
    for (T prev_value{}; prev_value != value;) {
        prev_value = value;
        value |= value >> 1;
    }
    return value;
}

template <typename T> struct to_ival {};

template <auto var_bits, auto const_bits>
struct to_ival<constrain_mask_t<var_bits, const_bits>> {
    /// lower bits that can change, including gaps between var_bits
    constexpr static auto var_portion = fill_in_bitmask(var_bits);

    /// fixed bits that will never change, excludes the var_portion
    constexpr static auto const_portion = const_bits & ~var_portion;

    constexpr static auto min = const_portion;
    constexpr static auto max = const_portion + (var_portion & var_bits);

    using type = constrain_interval_t<min, max>;
};

template <auto min, auto max> struct to_ival<constrain_interval_t<min, max>> {
    using type = constrain_interval_t<min, max>;
};

template <typename T> using to_ival_t = typename to_ival<T>::type;

[[nodiscard]] constexpr auto is_basic_mask(auto value) -> bool {
    return ((value >> 1) & value) == (value >> 1);
}

template <typename T> struct to_mask {};

template <auto var_bits, auto const_bits>
struct to_mask<constrain_mask_t<var_bits, const_bits>> {
    using type = constrain_mask_t<var_bits, const_bits>;
};

template <auto min, auto max> struct to_mask<constrain_interval_t<min, max>> {
    constexpr static auto var_bits = fill_in_bitmask(max ^ min);
    constexpr static auto const_bits = min & ~var_bits;
    using type = constrain_mask_t<var_bits, const_bits>;
};

template <typename T> using to_mask_t = typename to_mask<T>::type;
} // namespace detail

template <typename T> constexpr bool is_mask_v = false;

template <auto var_bits, auto const_bits>
constexpr bool is_mask_v<constrain_mask_t<var_bits, const_bits>> = true;

template <class T>
concept Mask = is_mask_v<T>;
} // namespace safe::dsl
