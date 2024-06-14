#pragma once

#include <safe/dsl/fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/constrain_mask.hpp>
#include <safe/dsl/primitive.hpp>

namespace safe::dsl {
template <typename T, typename U>
struct bitwise_or : public detail::binary_op {};

template <any_constraint LhsT, any_constraint RhsT>
[[nodiscard]] constexpr auto operator|(LhsT, RhsT) -> bitwise_or<LhsT, RhsT> {
    return {};
}

template <detail::Primitive LhsT, detail::Primitive RhsT>
struct bitwise_or<LhsT, RhsT> : public detail::binary_op {
    using lhs = detail::to_mask_t<LhsT>;
    using rhs = detail::to_mask_t<RhsT>;
    constexpr static auto value = lhs::value | rhs::value;
    using type = constrain_mask_t<value.var_bits(), value.const_bits()>;
};
} // namespace safe::dsl