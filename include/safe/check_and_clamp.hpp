#pragma once

#include <safe/contract.hpp>

namespace safe {
    template<typename T, typename... Requirements>
    struct check_and_clamp {
        contract<T, Requirements...> value_;

        constexpr check_and_clamp(
            contract<T, Requirements...> value
        )
            : value_{value}
        {}
    };
}