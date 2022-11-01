#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/fwd.hpp>

#include <algorithm>

namespace safe_dsl {
    namespace detail {
        template<typename T>
        [[nodiscard]] constexpr T abs(T value) {
            if (value < 0) {
                return -value;
            } else {
                return value;
            }
        }
    }

    template<typename T>
    struct abs_t {};

    template<auto min_val, auto max_val>
    struct abs_t<
        ival_t<min_val, max_val>
    > {
        constexpr static bool straddles_zero =
            min_val < 0 && max_val > 0;

        using type = ival_t<
            straddles_zero
                ? 0
                : std::min(detail::abs(min_val), detail::abs(max_val)),
            std::max(detail::abs(min_val), detail::abs(max_val))>;
    };

    template<typename T>
    [[nodiscard]] constexpr auto abs(T)
        -> abs_t<T>
    {
        return {};
    }
}