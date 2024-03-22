#pragma once

#include <safe/dsl/fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/constrain_mask.hpp>
#include <safe/dsl/primitive.hpp>

#include <algorithm>
#include <bit>

namespace safe::dsl {
template <typename T> struct bit_width_t {};

template <detail::Primitive T> struct bit_width_t<T> : public detail::unary_op {
    using val = detail::to_mask_t<T>;

    using type = constrain_interval_t<std::bit_width(val::const_bits),
                        std::bit_width(val::var_bits | val::const_bits)>;
};

template <any_constraint T>
[[nodiscard]] constexpr auto bit_width(T) -> bit_width_t<T> {
    return {};
}
} // namespace safe::dsl