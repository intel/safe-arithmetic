#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>

#include <type_traits>

namespace safe::dsl::detail {
    template <typename T>
    constexpr bool is_primitive_v = false;

    template <auto min, auto max>
    constexpr bool is_primitive_v<ival_t<min, max>> = true;

    template <auto var_bits, auto const_bits>
    constexpr bool is_primitive_v<mask_t<var_bits, const_bits>> = true;

    template<class T>
    concept Primitive = is_primitive_v<T>;
}