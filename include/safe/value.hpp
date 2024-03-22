#pragma once

#include <safe/dsl.hpp>
#include <safe/constrained_number.hpp>

#include <limits>

namespace safe {
constexpr auto value(auto value) {
    using value_t = decltype(value);
    constexpr value_t min = std::numeric_limits<value_t>::lowest();
    constexpr value_t max = std::numeric_limits<value_t>::max();
    return constraint_cast<constrain_interval<min, max>, value_t>(value);
}
} // namespace safe