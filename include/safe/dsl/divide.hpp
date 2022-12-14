#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>
#include <safe/dsl/detail/checked.hpp>

#include <algorithm>

namespace safe::dsl {
    template<typename T, typename U>
    struct divide : public binary_op {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct divide<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public binary_op
    {
        static_assert(
            (rhs_min < 0 && rhs_max < 0) ||
            (rhs_min > 0 && rhs_max > 0),
            "RHS of division operator must be guaranteed to not be '0'."
        );

        using type = ival_t<
            std::min({
                detail::c_<lhs_min> / detail::c_<rhs_min>,
                detail::c_<lhs_min> / detail::c_<rhs_max>,
                detail::c_<lhs_max> / detail::c_<rhs_min>,
                detail::c_<lhs_max> / detail::c_<rhs_max>
            }),
            std::max({
                detail::c_<lhs_min> / detail::c_<rhs_min>,
                detail::c_<lhs_min> / detail::c_<rhs_max>,
                detail::c_<lhs_max> / detail::c_<rhs_min>,
                detail::c_<lhs_max> / detail::c_<rhs_max>
            })
        >;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator/(LhsT, RhsT)
        -> divide<LhsT, RhsT>
    {
        return {};
    }
}