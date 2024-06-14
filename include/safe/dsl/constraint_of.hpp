#pragma once

#include <safe/dsl/constrain_interval.hpp>

#include <concepts>
#include <limits>

namespace safe::dsl {

template <std::integral T>
constexpr auto constraint_of = 
    constrain_interval<
        std::numeric_limits<T>::lowest(),
        std::numeric_limits<T>::max()
    >;

} // namespace safe::dsl