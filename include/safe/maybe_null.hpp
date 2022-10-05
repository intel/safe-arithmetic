#pragma once

namespace safe {

    struct maybe_null {
        template<typename T>
        constexpr static auto check(T value) {
            return true;
        }
    };
}