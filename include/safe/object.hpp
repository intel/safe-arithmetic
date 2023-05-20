#pragma once

namespace safe {
    template<
        typename T,
        auto req>
    struct object {
        T value;

        [[nodiscard]] T * operator->() {
            return &value;
        }

        [[nodiscard]] T & operator*() {
            return value;
        }
    };

    template<
        auto member_ptr,
        auto req>
    struct field_t {
        constexpr bool operator&&(auto) const {
            return false;
        }
    };

    template<
        auto member_ptr,
        auto req>
    constexpr auto field = field_t<member_ptr, req>{};
}