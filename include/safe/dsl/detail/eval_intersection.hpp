#pragma once

#include <safe/dsl/eval_fwd.hpp>
#include <safe/dsl/intersection.hpp>

#include <boost/mp11.hpp>

#include <type_traits>

namespace safe::dsl::detail {
using namespace boost::mp11;

template <typename IntervalList, typename NextInterval>
struct interval_intersection_merge {
    using prev_interval = mp_back<IntervalList>;

    using type =
        mp_push_back<mp_pop_back<IntervalList>,
                     ival_t<std::max(prev_interval::min, NextInterval::min),
                            std::min(prev_interval::max, NextInterval::max)>>;
};

template <typename NextInterval>
struct interval_intersection_merge<safe::dsl::intersection_t<>, NextInterval> {
    using type = safe::dsl::intersection_t<NextInterval>;
};

template <typename IntervalList, typename NextInterval>
using interval_intersection_merge_t =
    typename interval_intersection_merge<IntervalList, NextInterval>::type;

template <typename T0, typename T1, typename... Ts>
struct eval<safe::dsl::intersection_t<T0, T1, Ts...>> {
    using flat_intersection = mp_flatten<
        safe::dsl::intersection_t<eval_t<T0>, eval_t<T1>, eval_t<Ts>...>>;
    using sorted_intersection = mp_sort<flat_intersection, interval_less>;
    using merged_intersection =
        mp_fold<sorted_intersection, safe::dsl::intersection_t<>,
                interval_intersection_merge_t>;

    using type = mp_if<mp_same<mp_size<merged_intersection>, mp_size_t<1>>,
                       mp_first<merged_intersection>, merged_intersection>;
};

template <typename T> struct eval<safe::dsl::intersection_t<T>> {
    using type = T;
};
} // namespace safe::dsl::detail