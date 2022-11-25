#pragma once

#include <safe/detail/pure.hpp>

#include <type_traits>

namespace safe::dsl {
    template<auto Min, auto Max>
    struct ival_t {
        using type = ival_t;

        constexpr static auto min = Min;
        constexpr static auto max = Max;

        static_assert(min <= max);

        [[nodiscard]] SAFE_PURE constexpr static bool check(auto value) {
            return value >= min && value <= max;
        }
    };

    template<auto Min, auto Max>
    constexpr ival_t<Min, Max> ival{};


    template<typename T>
    struct is_ival : public std::integral_constant<bool, false> {};

    template<auto min, auto max>
    struct is_ival<ival_t<min, max>> : public std::integral_constant<bool, true> {};

    template<typename T>
    constexpr bool is_ival_v = is_ival<T>{};

    template<class T>
    concept Interval = is_ival_v<T>;
}