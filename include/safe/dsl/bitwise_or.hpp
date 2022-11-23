#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/fwd.hpp>

namespace safe::dsl {
    template<typename T, typename U>
    struct bitwise_or : public binary_op {};

    template<
        detail::Primitive LhsT,
        detail::Primitive RhsT>
    struct bitwise_or<LhsT, RhsT>
        : public binary_op
    {
        using lhs = detail::to_mask_t<LhsT>;
        using rhs = detail::to_mask_t<RhsT>;

        constexpr static auto lhs_const_bits = lhs::const_bits & ~lhs::var_bits;
        constexpr static auto rhs_const_bits = rhs::const_bits & ~rhs::var_bits;

        using type = mask_t<
            (
                (lhs::var_bits & ~rhs_const_bits) |
                (rhs::var_bits & ~lhs_const_bits)
            ),
            lhs_const_bits | rhs_const_bits
        >;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator|(LhsT, RhsT)
        -> bitwise_or<LhsT, RhsT>
    {
        return {};
    }
}