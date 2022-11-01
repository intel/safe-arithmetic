#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>

namespace safe_dsl {
    template<typename T, typename U>
    struct is_subset {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct is_subset<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    > {
        using type = is_subset;
        constexpr static bool value =
            lhs_min >= rhs_min &&
            lhs_max <= rhs_max;

        [[nodiscard]] constexpr operator bool() const {
            return value;
        }
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr bool operator<=(LhsT, RhsT) {
        return detail::simplify_v<is_subset<LhsT, RhsT>>;
    }
}