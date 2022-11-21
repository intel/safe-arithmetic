#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>

#include <type_traits>

namespace safe::dsl::detail {
    template<typename T>
    struct is_primitive : public std::integral_constant<bool, false> {};
    
    template<auto min, auto max>
    struct is_primitive<ival_t<min, max>> : public std::integral_constant<bool, true> {};

    template<auto var_bits, auto const_bits>
    struct is_primitive<mask_t<var_bits, const_bits>> : public std::integral_constant<bool, true> {};

    template<typename T>
    constexpr bool is_primitive_v = is_primitive<T>{};

    template<class T>
    concept Primitive = is_primitive_v<T>;
}