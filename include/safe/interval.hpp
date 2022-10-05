#pragma once

#include <boost/mp11.hpp>

#include <algorithm>
#include <type_traits>

namespace safe {
    using namespace boost::mp11;

    template<auto Min, auto Max>
    struct interval {
        constexpr static auto min = Min;
        constexpr static auto max = Max;

        static_assert(min <= max);

        template<typename T>
        constexpr static auto check(T value) {
            return value >= min && value <= max;
        }

        template<typename T>
        constexpr static auto clamp(T value) {
            return std::clamp<T>(value, min, max);
        }
    };


    template<typename LhsT, typename RhsT>
    struct interval_less;

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct interval_less<
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > : public std::integral_constant<bool,
        lhs_min < rhs_min
    > {};

    template<typename LhsT, typename RhsT>
    using binary_interval_merge = interval<
        std::min({LhsT::min, RhsT::min}),
        std::max({LhsT::max, RhsT::max})
    >;

    // assuming the intervals are sorted in ascending order by min
    template<typename LhsT, typename RhsT>
    using can_intervals_merge = std::integral_constant<bool,
        LhsT::max >= RhsT::min
    >;


    template<typename IntervalList, typename NextInterval>
    struct interval_merge {
        using prev_interval = mp_back<IntervalList>;
        using type = mp_if<can_intervals_merge<prev_interval, NextInterval>,
            // then...
            mp_push_back<mp_pop_back<IntervalList>, binary_interval_merge<prev_interval, NextInterval>>,
            // else...
            mp_push_back<IntervalList, NextInterval>
        >;
    };

    template<typename NextInterval>
    struct interval_merge<mp_list<>, NextInterval> {
        using type = mp_list<NextInterval>;
    };

    template<typename IntervalList, typename NextInterval>
    using interval_merge_t = typename interval_merge<IntervalList, NextInterval>::type;



    template<
        template<typename, typename> typename Operation,
        typename Lhs,
        typename Rhs>
    struct binary_interval_operation;

    template<template<typename, typename> typename Operation>
    struct binary_interval_operation_q {
        template<typename Lhs, typename Rhs>
        using operation_t = typename Operation<Lhs, Rhs>::type;

        template<typename Lhs, typename Rhs>
        using fn = typename binary_interval_operation<operation_t, Lhs, Rhs>::type;
    };

    template<
        template<typename, typename> typename Operation,
        typename... LhsIntervals,
        typename... RhsIntervals>
    struct binary_interval_operation<
        Operation,
        interval_union<LhsIntervals...>,
        interval_union<RhsIntervals...>
    > {
        using lhs_req = mp_list<LhsIntervals...>;
        using rhs_req = mp_list<RhsIntervals...>;
        using unsorted_result = mp_product<Operation, lhs_req, rhs_req>;
        using sorted_result = mp_sort<unsorted_result, interval_less>;
        using optimized_result = mp_fold<sorted_result, mp_list<>, interval_merge_t>;
        using type = mp_rename<optimized_result, interval_union>;

        constexpr static bool value = type::value;
    };

    template<
        template<typename, typename> typename Operation,
        auto lhs_min, auto lhs_max,
        typename... RhsIntervals>
    struct binary_interval_operation<
        Operation,
        interval<lhs_min, lhs_max>,
        interval_union<RhsIntervals...>
    > : public binary_interval_operation<
        Operation,
        interval_union<interval<lhs_min, lhs_max>>,
        interval_union<RhsIntervals...>
    > {};

    template<
        template<typename, typename> typename Operation,
        typename... LhsIntervals,
        auto rhs_min, auto rhs_max>
    struct binary_interval_operation<
        Operation,
        interval_union<LhsIntervals...>,
        interval<rhs_min, rhs_max>
    > : public binary_interval_operation<
        Operation,
        interval_union<LhsIntervals...>,
        interval_union<interval<rhs_min, rhs_max>>
    > {};

    template<
        template<typename, typename> typename Operation,
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct binary_interval_operation<
        Operation,
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > {
        using type = Operation<interval<lhs_min, lhs_max>, interval<rhs_min, rhs_max>>;
    };


    template<
        typename... LhsIntervals,
        typename... RhsIntervals>
    struct req_is_assignable<
        interval_union<LhsIntervals...>,
        interval_union<RhsIntervals...>
    > {
        using lhs_req = mp_list<LhsIntervals...>;
        using rhs_req = mp_list<RhsIntervals...>;
        using req_x_check = mp_product<req_is_assignable, lhs_req, rhs_req>;
        using type = mp_any_of<req_x_check, mp_to_bool>;

        constexpr static bool value = type::value;
    };

    template<
        typename... LhsIntervals,
        auto rhs_min, auto rhs_max>
    struct req_is_assignable<
        interval_union<LhsIntervals...>,
        interval<rhs_min, rhs_max>
    >
        : public req_is_assignable<
              interval_union<LhsIntervals...>,
              interval_union<interval<rhs_min, rhs_max>>
          >
    {};

    template<
        auto lhs_min, auto lhs_max,
        typename... RhsIntervals>
    struct req_is_assignable<
        interval<lhs_min, lhs_max>,
        interval_union<RhsIntervals...>
    > {
        using lhs_req = mp_list<interval<lhs_min, lhs_max>>;
        using rhs_req = mp_list<RhsIntervals...>;
        using req_x_check = mp_product<req_is_assignable, lhs_req, rhs_req>;
        using type = mp_all_of<req_x_check, mp_to_bool>;

        constexpr static bool value = type::value;
    };

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct req_is_assignable<
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > : public std::integral_constant<bool,
        rhs_min >= lhs_min &&
        rhs_max <= lhs_max
    > {};

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct req_operator_plus<
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > {
        using type = interval<lhs_min + rhs_min, lhs_max + rhs_max>;
    };



    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct req_operator_minus<
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > {
        using type = interval<lhs_min - rhs_max, lhs_max - rhs_min>;
    };

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct req_operator_multiply<
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > {
        constexpr static auto possible_values = {
            lhs_min * rhs_min,
            lhs_min * rhs_max,
            lhs_max * rhs_min,
            lhs_max * rhs_max
        };

        using type = interval<
            std::min(possible_values),
            std::max(possible_values)
        >;
    };

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct req_operator_divide<
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > {
        static_assert(
            (rhs_min < 0 && rhs_max < 0) ||
            (rhs_min > 0 && rhs_max > 0)
        );

        constexpr static auto possible_values = {
            lhs_min / rhs_min,
            lhs_min / rhs_max,
            lhs_max / rhs_min,
            lhs_max / rhs_max
        };

        using type = interval<
            std::min(possible_values),
            std::max(possible_values)
        >;
    };

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


        constexpr interval_value mod1(int64_t a, int64_t b, int64_t m) {
            // (2): compute modulo with positive interval and negate
            if (b < 0) {
                return -mod1(-b, -a, m);

            // (3): split into negative and non-negative interval, compute and join
            } else if (a < 0) {
                return mod1(a, -1, m) | mod1(0, b, m);

            // (4): there is no k > 0 such that a < k*m <= b
            } else if (
                ((b - a) < abs(m)) &&
                ((a % m) <= (b % m))
            ) {
                return {a % m, b % m};

            // (5): we can't do better than that
            } else {
                return {0, abs(m) - 1};
            }
        }

        constexpr interval_value mod2(int64_t a, int64_t b, int64_t m, int64_t n) {
            // (2): compute modulo with positive interval and negate
            if (b < 0) {
                return -mod2(-b, -a, m, n);

            // (3): split into negative and non-negative interval, compute, and join
            } else if (a < 0) {
                return mod2(a, -1, m, n) | mod2(0, b, m, n);

            // (4): use the simpler function from before
            } else if (m == n) {
                return mod1(a, b, m);

            // (5): use only non-negative m and n
            } else if (n <= 0) {
                return mod2(a, b, -n, -m);

            // (6): similar to (5), make modulus non-negative
            } else if (m <= 0) {
                return mod2(a, b, 1, std::max(-m, n));

            // (7): compare to (4) in mod1, check b-a < |modulus|
            } else if (b - a >= n) {
                return {0, n - 1};

            // (8): similar to (7), split interval, compute, and join
            } else if (b - a >= m) {
                return interval_value{0, b - a - 1} | mod2(a, b, b - a + 1, n);

            // (9): modulo has no effect
            } else if (m > b) {
                return {a, b};

            // (10): there is some overlapping of [a,b] and [n,m]
            } else if (n > b) {
                return {0, b};

            // (11): either compute all possibilities and join, or be imprecise
            } else {
                return {0, n - 1};
            }
        }
    }

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct req_operator_modulo<
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > {
        static_assert(
            (rhs_min < 0 && rhs_max < 0) ||
            (rhs_min > 0 && rhs_max > 0)
        );

        constexpr static detail::interval_value value =
            detail::mod2(lhs_min, lhs_max, rhs_min, rhs_max);

        using type = interval<value.min, value.max>;
    };


    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct req_operator_shift_left<
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > {
        constexpr static auto possible_values = {
            lhs_min << rhs_min,
            lhs_min << rhs_max,
            lhs_max << rhs_min,
            lhs_max << rhs_max
        };

        using type = interval<
            std::min(possible_values),
            std::max(possible_values)
        >;
    };

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct req_operator_shift_right<
        interval<lhs_min, lhs_max>,
        interval<rhs_min, rhs_max>
    > {
        constexpr static auto possible_values = {
            lhs_min >> rhs_min,
            lhs_min >> rhs_max,
            lhs_max >> rhs_min,
            lhs_max >> rhs_max
        };

        using type = interval<
            std::min(possible_values),
            std::max(possible_values)
        >;
    };
}