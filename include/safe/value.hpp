#pragma once

#include <safe/contract.hpp>
#include <safe/interval.hpp>

namespace safe {
    template<typename U>
    constexpr auto value(U value) {
        constexpr U min = std::numeric_limits<U>::min();
        constexpr U max = std::numeric_limits<U>::max();

        if constexpr (std::is_pointer_v<U>) {
            return contract<U, interval<min, max>, maybe_null>{value};

        } else {
            return contract<U, interval<min, max>>{value};
        }
    }
}