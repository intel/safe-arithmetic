#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/fwd.hpp>

namespace safe_dsl {
    template<typename T, typename U>
    struct add : public binary_op {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct add<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public binary_op
    {
        using type = ival_t<lhs_min + rhs_min, lhs_max + rhs_max>;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator+(LhsT, RhsT)
        -> add<LhsT, RhsT>
    {
        return {};
    }
}