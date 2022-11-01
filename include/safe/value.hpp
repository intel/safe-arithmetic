#pragma once

#include <safe/var.hpp>
#include <safe/dsl.hpp>

#include <limits>

namespace safe {
    template<typename U>
    constexpr auto value(U value) {
        constexpr U min = std::numeric_limits<U>::lowest();
        constexpr U max = std::numeric_limits<U>::max();
        return var<U, ival<min, max>>{value};
    }
}