#pragma once

#include <safe/dsl/union.hpp>
#include <safe/dsl/ival.hpp>

namespace safe::dsl {
    template<auto... Values>
    using set_t = union_t<ival_t<Values, Values>...>;

    template<auto... Values>
    constexpr set_t<Values...> set{};
}