#pragma once

#include <safe/dsl/fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/constrain_mask.hpp>
#include <safe/dsl/primitive.hpp>

namespace safe::dsl {
template <typename T, typename U> struct add : public detail::binary_op {};

template <Interval LhsT, Interval RhsT>
struct add<LhsT, RhsT> : public detail::binary_op {
    using type = constrain_interval_t<LhsT::min + RhsT::min, LhsT::max + RhsT::max>;
};

template <Mask LhsT, Mask RhsT>
struct add<LhsT, RhsT> : public detail::binary_op {
    constexpr static auto value = LhsT::value + RhsT::value;
    using type = constrain_mask_t<value.var_bits(), value.const_bits()>;
};

template <Operand LhsT, Operand RhsT>
[[nodiscard]] constexpr auto operator+(LhsT, RhsT) -> add<LhsT, RhsT> {
    return {};
}
} // namespace safe::dsl