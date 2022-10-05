#pragma once

#include <safe/contract.hpp>
#include <safe/interval.hpp>
#include <safe/maybe_null.hpp>
#include <safe/not_null.hpp>

namespace safe {
    template<typename U, U value>
    constexpr auto make_constant() {
        if constexpr (std::is_pointer_v<U>) {
            if constexpr (value == nullptr) {
                return contract<U, interval<value, value>, maybe_null>{value};
            } else {
                return contract<U, interval<value, value>, not_null>{value};
            }
        } else {
            return contract<U, interval<value, value>>{value};
        }
    }

    template<typename U, U value>
    constexpr contract<U, interval<value, value>> constant = make_constant<U, value>();
}