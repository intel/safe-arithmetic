#pragma once

#include <safe/dsl/eval_fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/constrain_mask.hpp>

namespace safe::dsl {
template <typename T, typename U> struct is_subset {};

template <auto lhs_min, auto lhs_max, auto rhs_min, auto rhs_max>
struct is_subset<constrain_interval_t<lhs_min, lhs_max>, constrain_interval_t<rhs_min, rhs_max>> {
    using type = is_subset;

    constexpr static bool value = lhs_min >= rhs_min && lhs_max <= rhs_max;

    [[nodiscard]] constexpr explicit operator bool() const { return value; }
};

template <auto lhs_variable_bits, auto lhs_constant_bits,
          auto rhs_variable_bits, auto rhs_constant_bits>
struct is_subset<constrain_mask_t<lhs_variable_bits, lhs_constant_bits>,
                 constrain_mask_t<rhs_variable_bits, rhs_constant_bits>> {
    using type = is_subset;

    constexpr static bool lhs_has_no_var_bits_outside_rhs_var_bits =
        ((~rhs_variable_bits & lhs_variable_bits) == 0);

    constexpr static bool lhs_const_bits_match_rhs_const_bits =
        ((~rhs_variable_bits & lhs_constant_bits) ==
         (~rhs_variable_bits & rhs_constant_bits));

    constexpr static bool value = lhs_has_no_var_bits_outside_rhs_var_bits &&
                                  lhs_const_bits_match_rhs_const_bits;

    [[nodiscard]] constexpr explicit operator bool() const { return value; }
};

template <auto lhs_min, auto lhs_max, auto rhs_variable_bits,
          auto rhs_constant_bits>
struct is_subset<constrain_interval_t<lhs_min, lhs_max>,
                 constrain_mask_t<rhs_variable_bits, rhs_constant_bits>> {
    using type = is_subset;

    using rhs_ival =
        detail::to_ival<constrain_mask_t<rhs_variable_bits, rhs_constant_bits>>;

    constexpr static bool value = detail::is_basic_mask(rhs_variable_bits) &&
                                  lhs_min >= rhs_ival::min &&
                                  lhs_max <= rhs_ival::max;

    [[nodiscard]] constexpr explicit operator bool() const { return value; }
};

template <auto lhs_variable_bits, auto lhs_constant_bits, auto rhs_min,
          auto rhs_max>
struct is_subset<constrain_mask_t<lhs_variable_bits, lhs_constant_bits>,
                 constrain_interval_t<rhs_min, rhs_max>> {
    using type = is_subset;

    using lhs_ival =
        detail::to_ival<constrain_mask_t<lhs_variable_bits, lhs_constant_bits>>;

    constexpr static bool value =
        lhs_ival::min >= rhs_min && lhs_ival::max <= rhs_max;

    [[nodiscard]] constexpr explicit operator bool() const { return value; }
};

template <any_constraint LhsT, any_constraint RhsT>
[[nodiscard]] constexpr auto operator<=(LhsT, RhsT) -> bool {
    return static_cast<bool>(detail::eval_v<is_subset<LhsT, RhsT>>);
}
} // namespace safe::dsl
