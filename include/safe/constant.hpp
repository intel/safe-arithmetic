#pragma once

#include <safe/dsl.hpp>
#include <safe/constrained_number.hpp>

namespace safe {
template <typename U, U value>
constexpr auto constant =
    constrained_number(std::integral_constant<U, value>{});
}