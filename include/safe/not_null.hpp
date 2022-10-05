#pragma once

#include <safe/maybe_null.hpp>

namespace safe {
    struct not_null {
        template<typename T>
        constexpr static bool check(T value) {
            return value != nullptr;
        }
    };

    template<>
    struct req_is_assignable<not_null, maybe_null>
        : public std::integral_constant<bool, false> {};

}