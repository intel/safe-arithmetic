#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/fwd.hpp>


namespace safe::dsl {
    template<typename T>
    struct bitwise_invert {};

    template<detail::Primitive T>
    struct bitwise_invert<T> {
        using mask_arg_t = detail::to_mask_t<T>;
        constexpr static auto value = ~mask_arg_t::value;
        using type = mask_t<value.var_bits(), value.const_bits()>;
    };

    template<typename T>
    [[nodiscard]] constexpr auto operator~(T)
        -> bitwise_invert<T>
    {
        return {};
    }
}