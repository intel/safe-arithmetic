#pragma once

#include <safe/var.hpp>
#include <safe/dsl.hpp>

namespace safe {
    template<typename U, U value>
    constexpr auto make_constant() {
        return var<U, ival<value, value>>{value};
    }

    template<typename U, U value>
    constexpr var<U, ival<value, value>> constant = make_constant<U, value>();
}