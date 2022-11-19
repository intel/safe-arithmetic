#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/fwd.hpp>

#include <safe/checked.hpp>

namespace safe::dsl {
    template<typename T, typename U>
    struct bitwise_or : public binary_op {};

    template<
        auto lhs_var_bits, auto lhs_const_bits,
        auto rhs_var_bits, auto rhs_const_bits>
    struct bitwise_or<
        mask_t<lhs_var_bits, lhs_const_bits>,
        mask_t<rhs_var_bits, rhs_const_bits>
    >
        : public binary_op
    {
        using type = mask_t<
            lhs_var_bits | rhs_var_bits,
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