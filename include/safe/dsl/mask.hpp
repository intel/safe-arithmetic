#pragma once

#include <type_traits>

namespace safe::dsl {
    template<auto AllowedBits>
    struct mask_t {
        using type = mask_t;

        constexpr static auto allowed_bits = AllowedBits;


        [[nodiscard]] constexpr static bool check(auto value) {
            return (~allowed_bits & value) == 0;
        }
    };

    template<auto AllowedBits>
    constexpr mask_t<AllowedBits> mask{};
}