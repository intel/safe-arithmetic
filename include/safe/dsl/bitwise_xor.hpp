#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/fwd.hpp>

namespace safe::dsl {
    template<typename T, typename U>
    struct bitwise_xor : public binary_op {};

    template<
        detail::Primitive LhsT,
        detail::Primitive RhsT>
    struct bitwise_xor<LhsT, RhsT>
        : public binary_op
    {
        using lhs = detail::to_mask_t<LhsT>;
        using rhs = detail::to_mask_t<RhsT>;

        using type = mask_t<
            lhs::var_bits | rhs::var_bits,
            lhs::const_bits ^ rhs::const_bits
        >;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator^(LhsT, RhsT)
        -> bitwise_xor<LhsT, RhsT>
    {
        return {};
    }
}