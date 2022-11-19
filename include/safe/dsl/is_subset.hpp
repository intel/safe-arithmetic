#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/simplify_fwd.hpp>

namespace safe::dsl {
    template<typename T, typename U>
    struct is_subset {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct is_subset<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    > {
        using type = is_subset;
        constexpr static bool value =
            lhs_min >= rhs_min &&
            lhs_max <= rhs_max;

        [[nodiscard]] constexpr operator bool() const {
            return value;
        }
    };

    template<
        auto lhs_variable_bits, auto lhs_constant_bits,
        auto rhs_variable_bits, auto rhs_constant_bits>
    struct is_subset<
        mask_t<lhs_variable_bits, lhs_constant_bits>,
        mask_t<rhs_variable_bits, rhs_constant_bits>
    > {
        using type = is_subset;

        constexpr static bool lhs_has_no_var_bits_outside_rhs_var_bits =
            ((~rhs_variable_bits & lhs_variable_bits) == 0);

        constexpr static bool lhs_const_bits_match_rhs_const_bits =
            ((~rhs_variable_bits & lhs_constant_bits) == (~rhs_variable_bits & rhs_constant_bits));

        constexpr static bool value =
            lhs_has_no_var_bits_outside_rhs_var_bits &&
            lhs_const_bits_match_rhs_const_bits;

        [[nodiscard]] constexpr operator bool() const {
            return value;
        }
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr bool operator<=(LhsT, RhsT) {
        return detail::simplify_v<is_subset<LhsT, RhsT>>;
    }
}