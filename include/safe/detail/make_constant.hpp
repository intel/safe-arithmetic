#pragma once


#include <safe/var.hpp>
#include <safe/dsl.hpp>


namespace safe::detail {
    template<typename U, U value>
    [[nodiscard]] inline constexpr auto make_constant() {
        return var<U, ival<value, value>>{value};
    }
}