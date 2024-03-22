#pragma once

#include <safe/constrained_number.hpp>

#include <stdexcept>
#include <type_traits>

namespace safe {
    auto const constraint_violation = 
        std::range_error("make_constrained constraint violation error");

    template<any_constraint auto C, std::integral T>
    auto make_constrained(T value) -> constrained_number<C, T> {
        if (C.check(value)) {
            return constraint_cast<C>(value);
        } else {
            throw constraint_violation;
        }
    }

    template<any_constraint auto C, any_constrained T>
    requires (T::constraint <= C)
    constexpr auto make_constrained(T value) -> T {
        return value;
    }

    template<any_constraint auto C, std::integral T, T V>
    requires (constrain_set<V> <= C)
    constexpr auto make_constrained(std::integral_constant<T, V> x) -> T {
        return constrained_number(x);
    }
}