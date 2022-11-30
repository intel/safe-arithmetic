#pragma once

#include <safe/var.hpp>
#include <safe/dsl.hpp>
#include <safe/detail/make_constant.hpp>

namespace safe {
    template<typename U, U value>
    constexpr var<U, ival<value, value>> constant = detail::make_constant<U, value>();
}