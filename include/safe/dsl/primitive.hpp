#pragma once

#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/constrain_mask.hpp>

#include <type_traits>

namespace safe::dsl::detail {
template <typename T> constexpr bool is_primitive_v = false;

template <auto min, auto max>
constexpr bool is_primitive_v<constrain_interval_t<min, max>> = true;

template <auto var_bits, auto const_bits>
constexpr bool is_primitive_v<constrain_mask_t<var_bits, const_bits>> = true;

template <class T>
concept Primitive = is_primitive_v<T>;
} // namespace safe::dsl::detail