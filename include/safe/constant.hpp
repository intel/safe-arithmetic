#pragma once

#include <safe/detail/make_constant.hpp>
#include <safe/dsl.hpp>
#include <safe/constrained_number.hpp>

namespace safe {
template <typename U, U value>
constexpr constrained_number<constrain_interval<value, value>, U> constant =
    detail::make_constant<U, value>();
}