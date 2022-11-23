#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/fwd.hpp>

#include <algorithm>

namespace safe::dsl {
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

    template<detail::Primitive T>
    struct abs_t<T> {
        using val = detail::to_ival_t<T>;
        
        constexpr static bool straddles_zero =
            val::min < 0 && val::max > 0;

        using type = ival_t<
            straddles_zero
                ? 0
                : std::min(detail::abs(val::min), detail::abs(val::max)),
            std::max(detail::abs(val::min), detail::abs(val::max))>;
    };

    template<typename T>
    [[nodiscard]] constexpr auto abs(T)
        -> abs_t<T>
    {
        return {};
    }
}