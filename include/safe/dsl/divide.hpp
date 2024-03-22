#pragma once

#include <safe/dsl/eval_fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>

#include <algorithm>

namespace safe::dsl {
template <typename T, typename U> struct divide : public detail::binary_op {};

template <auto lhs_min, auto lhs_max, auto rhs_min, auto rhs_max>
struct divide<constrain_interval_t<lhs_min, lhs_max>, constrain_interval_t<rhs_min, rhs_max>>
    : public detail::binary_op {
    static_assert((rhs_min < 0 && rhs_max < 0) || (rhs_min > 0 && rhs_max > 0),
                  "RHS of division operator must be guaranteed to not be '0'.");

    using type = constrain_interval_t<detail::min<lhs_min / rhs_min, lhs_min / rhs_max,
                                    lhs_max / rhs_min, lhs_max / rhs_max>,
                        detail::max<lhs_min / rhs_min, lhs_min / rhs_max,
                                    lhs_max / rhs_min, lhs_max / rhs_max>>;
};

template <any_constraint LhsT, any_constraint RhsT>
[[nodiscard]] constexpr auto operator/(LhsT, RhsT) -> divide<LhsT, RhsT> {
    return {};
}
} // namespace safe::dsl