#pragma once


#include <safe/var.hpp>


namespace safe {
    constexpr void for_each(
        Var auto begin,
        Var auto end,
        auto body
    ) {
        using int_t = decltype(begin.unsafe_value());

        for (int_t i = begin.unsafe_value(); i <= end.unsafe_value(); i++) {
            body(clamp(i, begin, end - constant<int_t, int_t{1}>));
        }
    }
}