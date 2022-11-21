#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/fwd.hpp>


namespace safe::dsl {
    template<typename T, typename U>
    struct bitwise_and : public binary_op {};

    template<
        detail::Primitive LhsT,
        detail::Primitive RhsT>
    struct bitwise_and<LhsT, RhsT>
        : public binary_op
    {
        using lhs_mask = detail::to_mask_t<LhsT>;
        using rhs_mask = detail::to_mask_t<RhsT>;

        using type = mask_t<
            (
                (lhs_mask::var_bits | lhs_mask::const_bits) &
                (rhs_mask::var_bits | rhs_mask::const_bits)
            ),
            lhs_mask::const_bits & rhs_mask::const_bits
        >;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator&(LhsT, RhsT)
        -> bitwise_and<LhsT, RhsT>
    {
        return {};
    }
}