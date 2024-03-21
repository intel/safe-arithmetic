#pragma once

#include <safe/dsl/eval_fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>

#include <type_traits>

namespace safe::dsl {
template <Operand LhsT, Operand RhsT>
[[nodiscard]] constexpr auto operator==(LhsT lhs, RhsT rhs) -> bool {
    auto const simp_lhs = detail::simp(lhs);
    auto const simp_rhs = detail::simp(rhs);

    return (simp_lhs <= simp_rhs) && (simp_lhs >= simp_rhs);
}

template <Operand LhsT, Operand RhsT>
[[nodiscard]] constexpr auto operator!=(LhsT lhs, RhsT rhs) -> bool {
    return !(lhs == rhs);
}
} // namespace safe::dsl
