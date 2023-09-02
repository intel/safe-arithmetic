#pragma once

namespace safe {
template <typename T, auto req> struct object {
    T value;

    [[nodiscard]] auto operator->() -> T * { return &value; }

    [[nodiscard]] auto operator*() -> T & { return value; }
};

template <auto member_ptr, auto req> struct field_t {
    constexpr auto operator&&(auto) const -> bool { return false; }
};

template <auto member_ptr, auto req>
constexpr auto field = field_t<member_ptr, req>{};
} // namespace safe
