#pragma once

#include <safe/detail/fwd.hpp>
#include <safe/dsl/constrain_interval.hpp>

#include <limits>
#include <type_traits>

namespace safe::detail {
template <typename T>
using integral_type = constrained_number<
    safe::dsl::constrain_interval<
        std::numeric_limits<T>::lowest(),
        std::numeric_limits<T>::max()>, 
    T>;
}