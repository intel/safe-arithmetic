#pragma once

#include <safe/constant.hpp>
#include <safe/detail/integral_type.hpp>
#include <safe/dsl.hpp>
#include <safe/constrained_number.hpp>

#include <cstdint>
#include <limits>

namespace safe {
namespace int_types {
template <int8_t value> constexpr auto s8_ = constant<int8_t, value>;
using s8 = detail::integral_type<int8_t>;

template <uint8_t value> constexpr auto u8_ = constant<uint8_t, value>;
using u8 = detail::integral_type<uint8_t>;

template <int16_t value> constexpr auto s16_ = constant<int16_t, value>;
using s16 = detail::integral_type<int16_t>;

template <uint16_t value> constexpr auto u16_ = constant<uint16_t, value>;
using u16 = detail::integral_type<uint16_t>;

template <uint32_t value> constexpr auto u32_ = constant<uint32_t, value>;
using u32 = detail::integral_type<uint32_t>;

template <int32_t value> constexpr auto s32_ = constant<int32_t, value>;
using s32 = detail::integral_type<int32_t>;

template <uint64_t value> constexpr auto u64_ = constant<uint64_t, value>;
using u64 = detail::integral_type<uint64_t>;

template <int64_t value> constexpr auto s64_ = constant<int64_t, value>;
using s64 = detail::integral_type<int64_t>;
} // namespace int_types

using namespace int_types;

namespace interval_types {
template <auto min, auto max> using ival_s8 = constrained_number<constrain_interval<min, max>, int8_t>;

template <auto min, auto max> using ival_u8 = constrained_number<constrain_interval<min, max>, uint8_t>;

template <auto min, auto max> using ival_s16 = constrained_number<constrain_interval<min, max>, int16_t>;

template <auto min, auto max> using ival_u16 = constrained_number<constrain_interval<min, max>, uint16_t>;

template <auto min, auto max> using ival_s32 = constrained_number<constrain_interval<min, max>, int32_t>;

template <auto min, auto max> using ival_u32 = constrained_number<constrain_interval<min, max>, uint32_t>;

template <auto min, auto max> using ival_s64 = constrained_number<constrain_interval<min, max>, int64_t>;

template <auto min, auto max> using ival_u64 = constrained_number<constrain_interval<min, max>, uint64_t>;
} // namespace interval_types

using namespace interval_types;

namespace detail {
template <typename T, char... Chars>
[[nodiscard]] constexpr auto to_constant() {
    // FIXME: handle or fail at compile-time for invalid strings
    // FIXME: select correct type ahead of time
    constexpr T value = []() {
        constexpr std::array<char, sizeof...(Chars)> chars{Chars...};
        T sum = 0;

        for (char c : chars) {
            T const digit = c - '0';
            sum = (sum * 10) + digit;
        }

        return sum;
    }();

    return make_constant<T, value>();
}
} // namespace detail

namespace literals {
template <char... Chars> constexpr auto operator""_cn() {
    return safe::detail::to_constant<int64_t, Chars...>();
}
} // namespace literals
} // namespace safe