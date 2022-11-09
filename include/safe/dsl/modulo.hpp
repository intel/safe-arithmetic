#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/simplify_fwd.hpp>

#include <cstdint>

namespace safe::dsl {
    template<typename T, typename U>
    struct modulo : public binary_op {};

    namespace detail {
        // https://stackoverflow.com/questions/31057473/calculating-the-modulo-of-two-intervals

        struct interval_value {
            int64_t min;
            int64_t max;
        };

        constexpr int64_t abs(int64_t v) {
            if (v < 0) {
                return -v;
            } else {
                return v;
            }
        }

        constexpr interval_value operator-(interval_value v) {
            return interval_value{-v.max, -v.min};
        }

        constexpr interval_value operator|(interval_value lhs, interval_value rhs) {
            return interval_value{
                std::min(lhs.min, rhs.min),
                std::max(lhs.max, rhs.max)
            };
        }


        constexpr interval_value mod1_body(
            interval_value lhs,
            int64_t rhs
        ) {
            // (4): there is no k > 0 such that lhs.min < k*rhs <= lhs.max
            if (
                ((lhs.max - lhs.min) < abs(rhs)) &&
                ((lhs.min % rhs) <= (lhs.max % rhs))
            ) {
                return {lhs.min % rhs, lhs.max % rhs};

            // (5): we can't do better than that
            } else {
                return {0, abs(rhs) - 1};
            }
        }

        constexpr interval_value mod1(
            interval_value lhs,
            int64_t rhs
        ) {
            // (2): compute modulo with positive ival and negate
            if (lhs.max < 0) {
                return -mod1_body(-lhs, rhs);

            // (3): split into negative and non-negative ival, compute and join
            } else if (lhs.min < 0) {
                return mod1_body({lhs.min, -1}, rhs) | mod1_body({0, lhs.max}, rhs);

            } else {
                return mod1_body(lhs, rhs);
            }
        }

        constexpr interval_value mod2(
            interval_value lhs,
            interval_value rhs
        ) {
            // (2): compute modulo with positive ival and negate
            if (lhs.max < 0) {
                return -mod2(-lhs, rhs);

            // (3): split into negative and non-negative ival, compute, and join
            } else if (lhs.min < 0) {
                return mod2({lhs.min, -1}, rhs) | mod2({0, lhs.max}, rhs);

            // (4): use the simpler function from before
            } else if (rhs.min == rhs.max) {
                return mod1(lhs, rhs.min);

            // (5): use only non-negative rhs
            } else if (rhs.max <= 0) {
                return mod2(lhs, -rhs);

            // (6): similar to (5), make modulus non-negative
            } else if (rhs.min <= 0) {
                return mod2(lhs, {1, std::max(-rhs.min, rhs.max)});

            // (7): compare to (4) in mod1, check lhs.max-lhs.min < |modulus|
            } else if (lhs.max - lhs.min >= rhs.max) {
                return {0, rhs.max - 1};

            // (8): similar to (7), split ival, compute, and join
            } else if (lhs.max - lhs.min >= rhs.min) {
                return interval_value{0, lhs.max - lhs.min - 1} | mod2(lhs, {lhs.max - lhs.min + 1, rhs.max});

            // (9): modulo has no effect
            } else if (rhs.min > lhs.max) {
                return lhs;

            // (10): there is some overlapping of lhs and rhs
            } else if (rhs.max > lhs.max) {
                return {0, lhs.max};

            // (11): either compute all possibilities and join, or be imprecise
            } else {
                return {0, rhs.max - 1};
            }
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
            detail::mod2({lhs_min, lhs_max}, {rhs_min, rhs_max});

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