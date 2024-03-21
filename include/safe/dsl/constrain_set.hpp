#pragma once

#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/union.hpp>

namespace safe::dsl {
template <auto... Values>
using constrain_set_t =
    union_t<constrain_interval_t<Values, Values>...>;

template <auto... Values> constexpr constrain_set_t<Values...> constrain_set{};
} // namespace safe::dsl