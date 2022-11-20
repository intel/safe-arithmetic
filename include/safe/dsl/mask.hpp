#pragma once

#include <safe/dsl/ival.hpp>

#include <type_traits>

namespace safe::dsl {
    template<auto VariableBits, auto ConstantBits = 0u>
    struct mask_t {
        using type = mask_t;

        constexpr static auto variable_bits = VariableBits;
        constexpr static auto constant_bits = ConstantBits;


        [[nodiscard]] constexpr static bool check(auto value) {
            return (~variable_bits & value) == constant_bits;
        }
    };

    template<auto VariableBits, auto ConstantBits = 0u>
    constexpr mask_t<VariableBits, ConstantBits> mask{};

    namespace detail {
        template<typename T>
        struct to_ival {};

        template<auto var_bits, auto const_bits>
        struct to_ival<mask_t<var_bits, const_bits>> {
            constexpr static auto min = const_bits & ~var_bits;
            constexpr static auto max = min + var_bits;
            using type = ival_t<min, max>;
        };

        template<auto min, auto max>
        struct to_ival<ival_t<min, max>> {
            using type = ival_t<min, max>;
        };

        template<typename T>
        using to_ival_t = typename to_ival<T>::type;


        template<auto var_bits, auto const_bits>
        [[nodiscard]] constexpr bool is_basic_mask(mask_t<var_bits, const_bits>) {
            return ((var_bits >> 1) & var_bits) == (var_bits >> 1);
        }
    }
}