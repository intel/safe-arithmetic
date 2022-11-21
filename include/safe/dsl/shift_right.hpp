#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>
#include <safe/checked.hpp>

#include <algorithm>

namespace safe::dsl {
    template<typename T, typename U>
    struct shift_right : public binary_op {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct shift_right<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public binary_op
    {
        using type = ival_t<
            std::min({
                c_<lhs_min> >> c_<rhs_min>,
                c_<lhs_min> >> c_<rhs_max>,
                c_<lhs_max> >> c_<rhs_min>,
                c_<lhs_max> >> c_<rhs_max>
            }),
            std::max({
                c_<lhs_min> >> c_<rhs_min>,
                c_<lhs_min> >> c_<rhs_max>,
                c_<lhs_max> >> c_<rhs_min>,
                c_<lhs_max> >> c_<rhs_max>
            })
        >;
    };

    template<
        auto lhs_var_bits, auto lhs_const_bits,
        auto rhs_val>
    struct shift_right<
        mask_t<lhs_var_bits, lhs_const_bits>,
        ival_t<rhs_val, rhs_val>
    >
        : public binary_op
    {
        using type = mask_t<
            operator>>(c_<lhs_var_bits>, c_<rhs_val>),
            operator>>(c_<lhs_const_bits>, c_<rhs_val>)
        >;
    };

    template<
        auto lhs_var_bits, auto lhs_const_bits,
        auto rhs_min, auto rhs_max>
    struct shift_right<
        mask_t<lhs_var_bits, lhs_const_bits>,
        ival_t<rhs_min, rhs_max>
    >
        : public binary_op
    {
        using lhs_mask =
            mask_t<lhs_var_bits, lhs_const_bits>;

        using type =
            bitwise_or<
                shift_right<lhs_mask, ival_t<rhs_min, rhs_max - 1>>,
                shift_right<lhs_mask, ival_t<rhs_max, rhs_max>>
            >;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator>>(LhsT, RhsT)
        -> shift_right<LhsT, RhsT>
    {
        return {};
    }
}