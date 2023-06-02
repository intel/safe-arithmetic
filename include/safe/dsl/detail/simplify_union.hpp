#pragma once

#include <safe/dsl/union.hpp>
#include <safe/dsl/simplify_fwd.hpp>
#include <safe/big_integer.hpp>

#include <boost/mp11.hpp>

#include <type_traits>

namespace safe::dsl::detail {
    using namespace boost::mp11;

    template<typename LhsT, typename RhsT>
    struct interval_less;

    template<
        auto lhs_min, auto lhs_max,
        auto rhs_min, auto rhs_max>
    struct interval_less<
        ival_t<lhs_min, lhs_max>,
        ival_t<rhs_min, rhs_max>
    > : public std::integral_constant<bool,
        lhs_min < rhs_min
    > {};

    template<typename LhsT, typename RhsT>
    using binary_interval_merge = ival_t<
        std::min<common_integer_t<decltype(LhsT::min), decltype(RhsT::min)> >(LhsT::min, RhsT::min),
        std::max<common_integer_t<decltype(LhsT::max), decltype(RhsT::max)> >(LhsT::max, RhsT::max)
    >;

    // assuming the intervals are sorted in ascending order by min
    // FIXME: assuming integral types
    template<typename LhsT, typename RhsT>
    using can_intervals_merge = std::integral_constant<bool,
        LhsT::max + 1 >= RhsT::min
    >;

    template<typename IntervalList, typename NextInterval>
    struct interval_union_merge {
        using prev_interval = mp_back<IntervalList>;
        using type = mp_if<can_intervals_merge<prev_interval, NextInterval>,
            // then...
            mp_push_back<mp_pop_back<IntervalList>, binary_interval_merge<prev_interval, NextInterval>>,
            // else...
            mp_push_back<IntervalList, NextInterval>
        >;
    };

    template<typename NextInterval>
    struct interval_union_merge<union_t<>, NextInterval> {
        using type = union_t<NextInterval>;
    };

    template<typename IntervalList, typename NextInterval>
    using interval_union_merge_t = typename interval_union_merge<IntervalList, NextInterval>::type;

    template<typename T0, typename T1, typename... Ts>
    struct simplify<union_t<T0, T1, Ts...>> {
        using flat_union = mp_flatten<union_t<simplify_t<T0>, simplify_t<T1>, simplify_t<Ts>...>>;
        using sorted_union = mp_sort<flat_union, interval_less>;
        using merged_union = mp_fold<sorted_union, union_t<>, interval_union_merge_t>;

        using type =
            mp_if_c<mp_size<merged_union>::value == 1,
                mp_first<merged_union>,
                merged_union
            >;
    };

    template<typename T>
    struct simplify<union_t<T>> {
        using type = T;
    };
}