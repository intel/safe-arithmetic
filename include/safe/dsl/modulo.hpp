#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>

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
    struct modulo : public binary_op {};

    namespace detail {
        struct interval_value {
            int64_t min;
            int64_t max;

            // empty interval
            constexpr interval_value()
                : min{1}
                , max{-1}
            {}

            constexpr interval_value(
                int64_t min_arg,
                int64_t max_arg
            )
                : min{min_arg}
                , max{max_arg}
            {}

            [[nodiscard]] constexpr bool empty() const {
                return min > max;
            }
        };

        [[nodiscard]] constexpr interval_value operator-(interval_value v) {
            return interval_value{-v.max, -v.min};
        }

        [[nodiscard]] constexpr int64_t abs(int64_t v) {
            if (v < 0) {
                return -v;
            } else {
                return v;
            }
        }

        [[nodiscard]] constexpr interval_value abs(interval_value v) {
            if (v.empty()) {
                return {};

            } else if (v.min >= 0 && v.max >= 0) {
                return v;

            } else if (v.min <= 0 && v.max <= 0) {
                return -v;

            } else {
                return {0, std::max(std::abs(v.min), std::abs(v.max))};
            }
        }

        [[nodiscard]] constexpr interval_value operator|(interval_value lhs, interval_value rhs) {
            if (lhs.empty()) {
                return rhs;

            } else if (rhs.empty()) {
                return lhs;

            } else {
                return interval_value{
                    std::min(lhs.min, rhs.min),
                    std::max(lhs.max, rhs.max)
                };
            }
        }

        [[nodiscard]] constexpr interval_value operator&(interval_value lhs, interval_value rhs) {
            if (lhs.empty() || rhs.empty()) {
                return {};
            } else {
                return interval_value{
                    std::max(lhs.min, rhs.min),
                    std::min(lhs.max, rhs.max)
                };
            }
        }

        /**
         * Get portion of lhs that is less than rhs.
         *
         * @param lhs
         * @param rhs
         * @return
         */
        [[nodiscard]] constexpr interval_value get_lower(
            interval_value lhs,
            interval_value rhs
        ) {
            return {lhs.min, std::min(lhs.max, rhs.min - 1)};
        }

        /**
         * Get portion of lhs that is greater than rhs.
         *
         * @param lhs
         * @param rhs
         * @return
         */
        [[nodiscard]] constexpr interval_value get_upper(
            interval_value lhs,
            interval_value rhs
        ) {
            return {std::max(lhs.min, rhs.max + 1), lhs.max};
        }

        [[nodiscard]] constexpr interval_value get_positive(interval_value v) {
            return {std::max(0ll, v.min), v.max};
        }

        [[nodiscard]] constexpr interval_value get_negative(interval_value v) {
            return {v.min, std::min(v.max, -1ll)};
        }

        [[nodiscard]] constexpr interval_value mod_lhs_low(
            interval_value lhs,
            interval_value rhs
        ) {
            if (lhs.empty() || rhs.empty()) {
                return {};
            } else {
                return lhs;
            }
        }

        [[nodiscard]] constexpr interval_value mod_lhs_high(
            interval_value lhs,
            interval_value rhs
        ) {
            // TODO: this could be smaller
            if (lhs.empty() || rhs.empty()) {
                return {};
            } else {
                return {0, rhs.max - 1};
            }
        }

        [[nodiscard]] constexpr interval_value mod_mid(
            interval_value mid
        ) {
            if (mid.empty()) {
                return {};
            } else {
                return interval_value{0, 0} | interval_value{mid.min, mid.max - 1};
            }
        }

        [[nodiscard]] constexpr interval_value pos_mod(
            interval_value lhs,
            interval_value rhs
        ) {
            if (lhs.empty() || rhs.empty()) {
                return {};
            } else {
                auto const lhs_low = get_lower(lhs, rhs);
                auto const lhs_high = get_upper(lhs, rhs);

                auto const mid = lhs & rhs;

                auto const rhs_low = get_lower(rhs, lhs);
                auto const rhs_high = get_upper(rhs, lhs);

                return
                    mod_lhs_low(lhs_low, mid) |
                    mod_lhs_low(lhs_low, rhs_high) |
                    mod_lhs_high(mid, rhs_low) |
                    mod_mid(mid) |
                    mod_lhs_low(mid, rhs_high) |
                    mod_lhs_high(lhs_high, rhs_low) |
                    mod_lhs_high(lhs_high, mid);
            }
        }

        [[nodiscard]] constexpr interval_value mod(
            interval_value lhs,
            interval_value rhs
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
        : public binary_op
    {
        static_assert(
            (rhs_min < 0 && rhs_max < 0) ||
            (rhs_min > 0 && rhs_max > 0),
            "RHS of modulo operator must be guaranteed to not be '0'."
        );

        constexpr static detail::interval_value value =
            detail::mod({lhs_min, lhs_max}, {rhs_min, rhs_max});

        using type = ival_t<value.min, value.max>;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator%(LhsT, RhsT)
        -> modulo<LhsT, RhsT>
    {
        return {};
    }
}