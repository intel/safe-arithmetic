#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/is_subset.hpp>
#include <safe/dsl/eval_fwd.hpp>

namespace safe::dsl {
    template<typename T, typename U>
    using is_superset = is_subset<U, T>;

    template<
        Operand LhsT,
        Operand RhsT>
    [[nodiscard]] constexpr bool operator>=(LhsT, RhsT) {
        return detail::eval_v<is_superset<LhsT, RhsT>>;
    }
}