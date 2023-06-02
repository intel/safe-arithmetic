#pragma once

#include <safe/dsl/union.hpp>
#include <safe/dsl/ival.hpp>
#include <safe/big_integer.hpp>

namespace safe::dsl {
    template<auto... Values>
    using set_t = union_t<ival_t<to_big_integer(Values), to_big_integer(Values)>...>;

    template<auto... Values>
    constexpr set_t<Values...> set{};
}