#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>

#include <type_traits>

namespace safe_dsl {
    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr bool operator==(LhsT, RhsT) {
        return std::is_same_v<
            detail::simplify_t<LhsT>,
            detail::simplify_t<RhsT>
        >;
    }

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr bool operator!=(LhsT lhs, RhsT rhs) {
        return !(lhs == rhs);
    }
}