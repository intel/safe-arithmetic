#pragma once

#include <safe/contract.hpp>
#include <safe/interval.hpp>

#include <algorithm>

namespace safe {
    template<typename T, T min, T max>
    constexpr auto clamp(
        T value
    ) -> contract<T, interval<min, max>> {
        return std::clamp<T>(value, min, max);
    };
}