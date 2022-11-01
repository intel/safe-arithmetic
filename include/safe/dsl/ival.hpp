#pragma once

#include <type_traits>

namespace safe_dsl {
    // FIXME: make sure these are the same integral type
    template<auto Min, auto Max>
    struct ival_t {
//        static_assert(std::is_same_v< decltype(Min), decltype(Max)>, "ival_t min and max values must have the same type.");
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