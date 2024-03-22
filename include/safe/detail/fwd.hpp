#pragma once

#ifndef SAFE_INLINE
#define SAFE_INLINE inline
#endif

namespace safe {
template <auto C, typename T> struct constrained_number;

template <typename T> constexpr bool is_constrained_number_v = false;

template <auto C, typename T>
constexpr bool is_constrained_number_v<constrained_number<C, T>> = true;

template <typename T>
concept any_constrained = is_constrained_number_v<T>;

[[nodiscard]] constexpr inline auto value(auto value);

namespace detail {
template <typename U, U value>
[[nodiscard]] constexpr inline auto make_constant();
}


} // namespace safe
