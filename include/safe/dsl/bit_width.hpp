#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/fwd.hpp>

#include <algorithm>
#include <bit>

namespace safe::dsl {
    template<typename T>
    struct bit_width_t {};

    template<detail::Primitive T>
    struct bit_width_t<T> {
        using val = detail::to_mask_t<T>;

        using type = ival_t<
            std::bit_width(val::const_bits),
            std::bit_width(val::var_bits | val::const_bits)
        >;
    };

    template<typename T>
    [[nodiscard]] constexpr auto bit_width(T)
        -> bit_width_t<T>
    {
        return {};
    }
}