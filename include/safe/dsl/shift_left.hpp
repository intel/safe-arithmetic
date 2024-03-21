#pragma once

#include <safe/dsl/bitwise_or.hpp>
#include <safe/dsl/eval_fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>

namespace safe::dsl {
template <typename T, typename U>
struct shift_left : public detail::binary_op {};

template <auto lhs_min, auto lhs_max, auto rhs_min, auto rhs_max>
struct shift_left<constrain_interval_t<lhs_min, lhs_max>, constrain_interval_t<rhs_min, rhs_max>>
    : public detail::binary_op {
    using type = constrain_interval_t<std::min({lhs_min << rhs_min, lhs_min << rhs_max,
                                  lhs_max << rhs_min, lhs_max << rhs_max}),
                        std::max({lhs_min << rhs_min, lhs_min << rhs_max,
                                  lhs_max << rhs_min, lhs_max << rhs_max})>;
};

template <auto lhs_var_bits, auto lhs_const_bits, auto rhs_min, auto rhs_max>
    requires(rhs_min == rhs_max)
struct shift_left<constrain_mask_t<lhs_var_bits, lhs_const_bits>,
                  constrain_interval_t<rhs_min, rhs_max>> : public detail::binary_op {
    using type = constrain_mask_t<lhs_var_bits << rhs_max, lhs_const_bits << rhs_max>;
};

template <auto lhs_var_bits, auto lhs_const_bits, auto rhs_min, auto rhs_max>
    requires(rhs_min < rhs_max)
struct shift_left<constrain_mask_t<lhs_var_bits, lhs_const_bits>,
                  constrain_interval_t<rhs_min, rhs_max>> : public detail::binary_op {
    using lhs_mask = constrain_mask_t<lhs_var_bits, lhs_const_bits>;

    using type = bitwise_or<shift_left<lhs_mask, constrain_interval_t<rhs_min, rhs_max - 1>>,
                            shift_left<lhs_mask, constrain_interval_t<rhs_max, rhs_max>>>;
};

template <Operand LhsT, Operand RhsT>
[[nodiscard]] constexpr auto operator<<(LhsT, RhsT) -> shift_left<LhsT, RhsT> {
    return {};
}
} // namespace safe::dsl