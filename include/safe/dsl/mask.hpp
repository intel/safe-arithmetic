#pragma once

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
}