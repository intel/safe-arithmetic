#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>
#include <safe/checked.hpp>

namespace safe::dsl {
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
                c_<lhs_min> << c_<rhs_min>,
                c_<lhs_min> << c_<rhs_max>,
                c_<lhs_max> << c_<rhs_min>,
                c_<lhs_max> << c_<rhs_max>
            }),
            std::max({
                c_<lhs_min> << c_<rhs_min>,
                c_<lhs_min> << c_<rhs_max>,
                c_<lhs_max> << c_<rhs_min>,
                c_<lhs_max> << c_<rhs_max>
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