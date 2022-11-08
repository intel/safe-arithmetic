#pragma once

#include <type_traits>

namespace safe_dsl {
    template<auto Min, auto Max>
    struct ival_t {
        using type = ival_t;

        constexpr static auto min = Min;
        constexpr static auto max = Max;

        static_assert(min <= max);

        [[nodiscard]] constexpr static bool check(auto value) {
            return value >= min && value <= max;
        }
    };

    template<auto Min, auto Max>
    constexpr ival_t<Min, Max> ival{};
}