#pragma once

#include <safe/dsl/eval_fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/constrain_mask.hpp>

namespace safe::dsl {
template <typename T, typename U> struct minus : public detail::binary_op {};

template <Interval LhsT, Interval RhsT>
struct minus<LhsT, RhsT> : public detail::binary_op {
    using type = constrain_interval_t<LhsT::min - RhsT::max, LhsT::max - RhsT::min>;
};

template <Mask LhsT, Mask RhsT>
struct minus<LhsT, RhsT> : public detail::binary_op {
    constexpr static auto value = LhsT::value - RhsT::value;
    using type = constrain_mask_t<value.var_bits(), value.const_bits()>;
};

template <any_constraint LhsT, any_constraint RhsT>
[[nodiscard]] constexpr auto operator-(LhsT, RhsT) -> minus<LhsT, RhsT> {
    return {};
}
} // namespace safe::dsl