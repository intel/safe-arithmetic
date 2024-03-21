#pragma once

#include <safe/dsl/fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>

#include <algorithm>

namespace safe::dsl {
template <typename T, typename U> struct min_t : public detail::binary_op {};

template <auto lhs_min, auto lhs_max, auto rhs_min, auto rhs_max>
struct min_t<constrain_interval_t<lhs_min, lhs_max>, constrain_interval_t<rhs_min, rhs_max>>
    : public detail::binary_op {
    using type = constrain_interval_t<
        std::min<std::common_type_t<decltype(lhs_min), decltype(rhs_min)>>(
            lhs_min, rhs_min),
        std::min<std::common_type_t<decltype(lhs_max), decltype(rhs_max)>>(
            lhs_max, rhs_max)>;
};

template <Operand LhsT, Operand RhsT>
[[nodiscard]] constexpr auto min(LhsT, RhsT) -> min_t<LhsT, RhsT> {
    return {};
}
} // namespace safe::dsl