#pragma once

#include <concepts>

namespace safe::detail {
    template<class I>
    concept iter_like =
        requires(I i) {
            i++;
            *i;
            i != i;
        };

    template<class R>
    concept range_like =
        requires(R r) {
            {r.begin()} -> iter_like;
            r.end();
        };
}