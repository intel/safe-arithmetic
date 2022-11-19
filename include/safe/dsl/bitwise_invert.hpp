#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/fwd.hpp>

#include <safe/checked.hpp>

namespace safe::dsl {
    template<typename T>
    struct bitwise_invert {};

    template<auto mask_value, auto fixed_value>
    struct bitwise_invert<
        mask_t<mask_value, fixed_value>
    > {
        using type = mask_t<mask_value, ~fixed_value>;
    };

    template<typename T>
    [[nodiscard]] constexpr auto operator~(T)
        -> bitwise_invert<T>
    {
        return {};
    }
}