#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>

#include <type_traits>

namespace safe::dsl {
    template<
        Operand LhsT,
        Operand RhsT>
    [[nodiscard]] constexpr bool operator==(LhsT lhs, RhsT rhs) {
        auto const simp_lhs = detail::simp(lhs);
        auto const simp_rhs = detail::simp(rhs);

        return
            (simp_lhs <= simp_rhs) &&
            (simp_lhs >= simp_rhs);
    }

    template<
        Operand LhsT,
        Operand RhsT>
    [[nodiscard]] constexpr bool operator!=(LhsT lhs, RhsT rhs) {
        return !(lhs == rhs);
    }
}