#pragma once


#include <safe/var.hpp>


namespace safe {
    constexpr void for_each(
        Var auto first,
        Var auto last,
        auto body
    ) {
        using int_t = decltype(first.unsafe_value());

        for (int_t i = first.unsafe_value(); i <= last.unsafe_value(); i++) {
            body(clamp(i, first, last));
        }
    }
}