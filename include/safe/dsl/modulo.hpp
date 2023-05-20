#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>
#include <safe/big_integer.hpp>

#include <cstdint>


/*
 * C++ `%` operator is not modulo, but it is close.
 *
 * a % -b == a % b
 * -a % b = -(a % b)
 *
 * return [0, rhs.max - 1] (worst case)
 *                                   -0+                       |-- lhs --|
 *                                   -0+            |-- rhs --|
 *
 * return [0, lhs.max - lhs.min] u [lhs.min, lhs.max - 1]
 *                                   -0+            |-- lhs --|
 *                                   -0+            |-- rhs --|
 *
 * return lhs
 *                                   -0+ |-- lhs --|
 *                                   -0+            |-- rhs --|
 *
 * return lhs
 *                       |-- lhs --| -0+ :   lhs   :
 *            :   rhs   :            -0+            |-- rhs --|
 *
 * return [abs(lhs.max - lhs.min), 0] u [lhs.min + 1, lhs.max] // [lhs.min + 1, 0]
 *            |-- lhs --|            -0+            :   lhs   :
 *            :   rhs   :            -0+            |-- rhs --|
 *
 * return -[0, rhs.max - 1] (worst case)
 * |-- lhs --|                       -0+                       :   lhs   :
 *            :   rhs   :            -0+            |-- rhs --|
 *
 */

namespace safe::dsl {
    template<typename T, typename U>
    struct modulo : public detail::binary_op {};

    namespace detail {
        template<typename Min, typename Max>
        struct interval_value {
            Min min;
            Max max;

            // empty interval
            constexpr interval_value()
                : min{1_i}
                , max{-1_i}
            {}

            constexpr interval_value(
                Min min_arg,
                Max max_arg
            )
                : min{min_arg}
                , max{max_arg}
            {}

            [[nodiscard]] constexpr bool empty() const {
                return min > max;
            }
        };

        template<typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto operator-(interval_value<RhsMin, RhsMax> v) {
            return interval_value{-v.max, -v.min};
        }

        template<std::size_t NumBits>
        [[nodiscard]] constexpr auto abs(big_integer<NumBits> v) {
            if (v < 0_i) {
                return -v;
            } else {
                return decltype(-v){v};
            }
        }

        template<typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr interval_value<RhsMin, RhsMax> abs(interval_value<RhsMin, RhsMax> v) {
            if (v.empty()) {
                return interval_value<RhsMin, RhsMax>{};

            } else if (v.min >= 0_i && v.max >= 0_i) {
                return v;

            } else if (v.min <= 0_i && v.max <= 0_i) {
                return interval_value<RhsMin, RhsMax>{
                    unsafe_cast<RhsMin>(-v.max),
                    unsafe_cast<RhsMax>(-v.min)
                };

            } else {
                return interval_value<RhsMin, RhsMax>{
                    0_i,
                    unsafe_cast<RhsMax>(safe::max(abs(v.min), abs(v.max)))
                };
            }
        }

        template<
            typename LhsMin, typename LhsMax,
            typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto operator|(
            interval_value<LhsMin, LhsMax> lhs,
            interval_value<RhsMin, RhsMax> rhs
        ) {
            auto value = interval_value{
                safe::min(lhs.min, rhs.min),
                safe::max(lhs.max, rhs.max)
            };

            using value_t = decltype(value);

            if (lhs.empty()) {
                return value_t{rhs.min, rhs.max};

            } else if (rhs.empty()) {
                return value_t{lhs.min, lhs.max};

            } else {
                return interval_value{
                    safe::min(lhs.min, rhs.min),
                    safe::max(lhs.max, rhs.max)
                };
            }
        }

        template<
            typename LhsMin, typename LhsMax,
            typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto operator&(
            interval_value<LhsMin, LhsMax> lhs,
            interval_value<RhsMin, RhsMax> rhs
        ) {
            auto retval =
                interval_value{
                    safe::max(lhs.min, rhs.min),
                    safe::min(lhs.max, rhs.max)
                };

            if (lhs.empty() || rhs.empty()) {
                return decltype(retval){};

            } else {
                return retval;
            }
        }

        /**
         * Get portion of lhs that is less than rhs.
         *
         * @param lhs
         * @param rhs
         * @return
         */
        template<
            typename LhsMin, typename LhsMax,
            typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto get_lower(
            interval_value<LhsMin, LhsMax> lhs,
            interval_value<RhsMin, RhsMax> rhs
        ) {
            return interval_value{lhs.min, safe::min(lhs.max, rhs.min - 1_i)};
        }

        /**
         * Get portion of lhs that is greater than rhs.
         *
         * @param lhs
         * @param rhs
         * @return
         */
        template<
            typename LhsMin, typename LhsMax,
            typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto get_upper(
            interval_value<LhsMin, LhsMax> lhs,
            interval_value<RhsMin, RhsMax> rhs
        ) {
            return interval_value{safe::max(lhs.min, rhs.max + 1_i), lhs.max};
        }

        template<typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto get_positive(interval_value<RhsMin, RhsMax> v) {
            return interval_value{safe::max(0_i, v.min), v.max};
        }

        template<typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto get_negative(interval_value<RhsMin, RhsMax> v) {
            return interval_value{v.min, safe::min(v.max, -1_i)};
        }

        template<
            typename LhsMin, typename LhsMax,
            typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto mod_lhs_low(
            interval_value<LhsMin, LhsMax> lhs,
            interval_value<RhsMin, RhsMax> rhs
        ) {
            if (lhs.empty() || rhs.empty()) {
                return interval_value<LhsMin, LhsMax>{};
            } else {
                return lhs;
            }
        }

        template<
            typename LhsMin, typename LhsMax,
            typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto mod_lhs_high(
            interval_value<LhsMin, LhsMax> lhs,
            interval_value<RhsMin, RhsMax> rhs
        ) {
            // TODO: this could be smaller
            auto retval = interval_value{0_i, rhs.max - 1_i};

            if (lhs.empty() || rhs.empty()) {
                return decltype(retval){};
            } else {
                return retval;
            }
        }

        template<typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto mod_mid(
            interval_value<RhsMin, RhsMax> mid
        ) {
            auto retval = interval_value{0_i, 0_i} | interval_value{mid.min, mid.max - 1_i};

            if (mid.empty()) {
                return decltype(retval){};
            } else {
                return retval;
            }
        }

        template<
            typename LhsMin, typename LhsMax,
            typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto pos_mod(
            interval_value<LhsMin, LhsMax> lhs,
            interval_value<RhsMin, RhsMax> rhs
        ) {
            auto const lhs_low = get_lower(lhs, rhs);
            auto const lhs_high = get_upper(lhs, rhs);

            auto const mid = lhs & rhs;

            auto const rhs_low = get_lower(rhs, lhs);
            auto const rhs_high = get_upper(rhs, lhs);

            auto retval =
                mod_lhs_low(lhs_low, mid) |
                mod_lhs_low(lhs_low, rhs_high) |
                mod_lhs_high(mid, rhs_low) |
                mod_mid(mid) |
                mod_lhs_low(mid, rhs_high) |
                mod_lhs_high(lhs_high, rhs_low) |
                mod_lhs_high(lhs_high, mid);

            if (lhs.empty() || rhs.empty()) {
                return decltype(retval){};
            } else {
                return retval;
            }
        }

        template<
            typename LhsMin, typename LhsMax,
            typename RhsMin, typename RhsMax>
        [[nodiscard]] constexpr auto mod(
            interval_value<LhsMin, LhsMax> lhs,
            interval_value<RhsMin, RhsMax> rhs
        ) {
            auto const pos_lhs = get_positive(lhs);
            auto const neg_lhs = get_negative(lhs);

            auto const pos_rhs = abs(rhs);

            return
                pos_mod(pos_lhs, pos_rhs) |
                -pos_mod(-neg_lhs, pos_rhs);
        }
    }

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct modulo<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    >
        : public detail::binary_op
    {
        static_assert(
            (rhs_min < 0 && rhs_max < 0) ||
            (rhs_min > 0 && rhs_max > 0),
            "RHS of modulo operator must be guaranteed to not be '0'."
        );

        constexpr static detail::interval_value value =
            detail::mod(
                detail::interval_value{lhs_min, lhs_max},
                detail::interval_value{rhs_min, rhs_max});

        using type = ival_t<value.min, value.max>;
    };

    template<
        Operand LhsT,
        Operand RhsT>
    [[nodiscard]] constexpr auto operator%(LhsT, RhsT)
        -> modulo<LhsT, RhsT>
    {
        return {};
    }
}