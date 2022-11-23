#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/fwd.hpp>
#include <safe/checked.hpp>

namespace safe::dsl {
    template<typename T, typename U>
    struct add : public binary_op {};

    template<
        detail::Primitive LhsT,
        detail::Primitive RhsT>
    struct add<LhsT, RhsT>
        : public binary_op
    {
        using lhs = detail::to_ival_t<LhsT>;
        using rhs = detail::to_ival_t<RhsT>;

        using type = ival_t<
            c_<lhs::min> + c_<rhs::min>,
            c_<lhs::max> + c_<rhs::max>
        >;
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