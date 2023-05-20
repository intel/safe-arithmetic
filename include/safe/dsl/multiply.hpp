#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/fwd.hpp>

#include <algorithm>

namespace safe::dsl {
    template<typename T, typename U>
    struct multiply : public detail::binary_op {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct multiply<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public detail::binary_op
    {

        using type = ival_t<
            detail::min<
                lhs_min * rhs_min,
                lhs_min * rhs_max,
                lhs_max * rhs_min,
                lhs_max * rhs_max
            >,
            detail::max<
                lhs_min * rhs_min,
                lhs_min * rhs_max,
                lhs_max * rhs_min,
                lhs_max * rhs_max
            >
        >;
    };

    template<
        Operand LhsT,
        Operand RhsT>
    [[nodiscard]] constexpr auto operator*(LhsT, RhsT)
        -> multiply<LhsT, RhsT>
    {
        return {};
    }
}