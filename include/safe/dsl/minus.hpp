#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>
#include <safe/dsl/detail/checked.hpp>

namespace safe::dsl {
    template<typename T, typename U>
    struct minus : public binary_op {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct minus<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public binary_op
    {
        using type = ival_t<
            detail::c_<lhs_min> - detail::c_<rhs_max>,
            detail::c_<lhs_max> - detail::c_<rhs_min>
        >;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator-(LhsT, RhsT)
        -> minus<LhsT, RhsT>
    {
        return {};
    }
}