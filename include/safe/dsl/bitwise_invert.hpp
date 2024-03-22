#pragma once

#include <safe/dsl/fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/constrain_mask.hpp>
#include <safe/dsl/primitive.hpp>

namespace safe::dsl {
template <typename T> struct bitwise_invert {};

template <detail::Primitive T>
struct bitwise_invert<T> : public detail::unary_op {
    using mask_arg_t = detail::to_mask_t<T>;
    constexpr static auto value = ~mask_arg_t::value;
    using type = constrain_mask_t<value.var_bits(), value.const_bits()>;
};

template <any_constraint T>
[[nodiscard]] constexpr auto operator~(T) -> bitwise_invert<T> {
    return {};
}
} // namespace safe::dsl