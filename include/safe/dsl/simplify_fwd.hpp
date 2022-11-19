#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/union.hpp>
#include <safe/dsl/intersection.hpp>

#include <boost/mp11.hpp>

namespace safe::dsl::detail {
    using namespace boost::mp11;
    
    template<typename T, typename Enable = void>
    struct simplify {
        using type = typename T::type;
        constexpr static type value{};
    };

    template<typename T>
    using simplify_t = typename simplify<T>::type;

    template<typename T>
    constexpr auto simplify_v = simplify<T>::value;

    template<typename T>
    [[nodiscard]] constexpr auto simp(T) {
        return simplify_t<T>{};
    }

    template<typename T>
    struct is_primitive : public std::integral_constant<bool, false> {};
    
    template<auto min, auto max>
    struct is_primitive<ival_t<min, max>> : public std::integral_constant<bool, true> {};

    template<auto var_bits, auto const_bits>
    struct is_primitive<mask_t<var_bits, const_bits>> : public std::integral_constant<bool, true> {};

    template<typename T>
    constexpr bool is_primitive_v = is_primitive<T>{};



    template<typename T>
    struct is_union : public std::integral_constant<bool, false> {};

    template<typename... Ts>
    struct is_union<union_t<Ts...>> : public std::integral_constant<bool, true> {};

    template<typename T>
    constexpr bool is_union_v = is_union<T>{};
}