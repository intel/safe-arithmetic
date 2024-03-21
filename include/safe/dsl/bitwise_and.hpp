#pragma once

#include <safe/dsl/fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/constrain_mask.hpp>
#include <safe/dsl/primitive.hpp>

namespace safe::dsl {
template <typename T, typename U>
struct bitwise_and : public detail::binary_op {};

template <detail::Primitive LhsT, detail::Primitive RhsT>
struct bitwise_and<LhsT, RhsT> : public detail::binary_op {
    using lhs = detail::to_mask_t<LhsT>;
    using rhs = detail::to_mask_t<RhsT>;
    constexpr static auto value = lhs::value & rhs::value;
    using type = constrain_mask_t<value.var_bits(), value.const_bits()>;
};

template <Operand LhsT, Operand RhsT>
[[nodiscard]] constexpr auto operator&(LhsT, RhsT) -> bitwise_and<LhsT, RhsT> {
    return {};
}
} // namespace safe::dsl