#pragma once

#include <safe/big_integer.hpp>
#include <safe/dsl/ival.hpp>
#include <safe/dsl/union.hpp>

namespace safe::dsl {
template <auto... Values>
using set_t =
    union_t<ival_t<to_big_integer(Values), to_big_integer(Values)>...>;

template <auto... Values> constexpr set_t<Values...> set{};
} // namespace safe::dsl