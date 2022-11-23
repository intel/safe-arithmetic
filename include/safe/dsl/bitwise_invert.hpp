#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/fwd.hpp>

#include <safe/checked.hpp>

namespace safe::dsl {
    template<typename T>
    struct bitwise_invert {};

    template<detail::Primitive T>
    struct bitwise_invert<T> {
        using prim_mask = detail::to_mask_t<T>;
        using type = mask_t<prim_mask::var_bits, ~prim_mask::const_bits>;
    };

    template<typename T>
    [[nodiscard]] constexpr auto operator~(T)
        -> bitwise_invert<T>
    {
        return {};
    }
}