#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/fwd.hpp>

#include <algorithm>

namespace safe::dsl {
    template<typename T, typename U>
    struct max_t : public binary_op {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct max_t<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public binary_op
    {
        using type = ival_t<
            std::max(lhs_min, rhs_min),
            std::max(lhs_max, rhs_max)>;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto max(LhsT, RhsT)
        -> max_t<LhsT, RhsT>
    {
        return {};
    }
}