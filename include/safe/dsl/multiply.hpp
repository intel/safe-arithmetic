#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/fwd.hpp>
#include <safe/checked.hpp>

#include <algorithm>

namespace safe::dsl {
    template<typename T, typename U>
    struct multiply : public binary_op {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct multiply<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public binary_op
    {
        using type = ival_t<
            std::min({
                c_<lhs_min> * c_<rhs_min>,
                c_<lhs_min> * c_<rhs_max>,
                c_<lhs_max> * c_<rhs_min>,
                c_<lhs_max> * c_<rhs_max>
            }),
            std::max({
                c_<lhs_min> * c_<rhs_min>,
                c_<lhs_min> * c_<rhs_max>,
                c_<lhs_max> * c_<rhs_min>,
                c_<lhs_max> * c_<rhs_max>
            })
        >;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator*(LhsT, RhsT)
        -> multiply<LhsT, RhsT>
    {
        return {};
    }
}