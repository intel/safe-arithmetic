#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>

namespace safe_dsl {
    // FIXME: this is not correct :(
    template<typename T, typename U>
    struct shift_left : public binary_op {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct shift_left<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public binary_op
    {
        using type = ival_t<
            std::min({
                lhs_min << rhs_min,
                lhs_min << rhs_max,
                lhs_max << rhs_min,
                lhs_max << rhs_max
            }),
            std::max({
                lhs_min << rhs_min,
                lhs_min << rhs_max,
                lhs_max << rhs_min,
                lhs_max << rhs_max
            })
        >;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator<<(LhsT, RhsT)
        -> shift_left<LhsT, RhsT>
    {
        return {};
    }
}