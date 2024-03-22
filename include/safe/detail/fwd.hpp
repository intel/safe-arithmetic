#pragma once


#include <safe/dsl/fwd.hpp>


#ifndef SAFE_INLINE
#define SAFE_INLINE inline
#endif

namespace safe {
template <any_constraint auto C, typename T> struct constrained_number;

template <typename T> constexpr bool is_constrained_number_v = false;

template <any_constraint auto C, typename T>
constexpr bool is_constrained_number_v<constrained_number<C, T>> = true;

template <typename T>
concept any_constrained = is_constrained_number_v<T>;



} // namespace safe
