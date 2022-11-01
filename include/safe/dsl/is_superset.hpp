#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/is_subset.hpp>
#include <safe/dsl/simplify_fwd.hpp>

namespace safe_dsl {
    template<typename T, typename U>
    using is_superset = is_subset<U, T>;

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr bool operator>=(LhsT, RhsT) {
        return detail::simplify_v<is_superset<LhsT, RhsT>>;
    }
}