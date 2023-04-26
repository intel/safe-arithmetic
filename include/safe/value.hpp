#pragma once

#include <safe/var.hpp>
#include <safe/dsl.hpp>

#include <limits>

namespace safe {
    constexpr auto value(auto value) {
        using value_t = decltype(value);
        constexpr value_t min = std::numeric_limits<value_t>::lowest();
        constexpr value_t max = std::numeric_limits<value_t>::max();
        return unsafe_cast<var<value_t, ival<min, max>>>(value);
    }
}