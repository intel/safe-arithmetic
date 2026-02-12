#pragma once

#ifndef SAFE_INLINE
#define SAFE_INLINE inline
#endif

namespace safe {
template <typename T, auto Requirement> struct var;

template <typename T> constexpr bool is_var_v = false;

template <typename T, auto Requirement>
constexpr bool is_var_v<var<T, Requirement>> = true;

template <typename T>
concept Var = is_var_v<T>;

[[nodiscard]] constexpr inline auto value(auto value);

namespace detail {
template <typename U, U value>
[[nodiscard]] constexpr inline auto make_constant();
}

template <typename T> struct unsafe_cast_ferry {
  private:
    T v;

  public:
    SAFE_INLINE constexpr explicit(true) unsafe_cast_ferry(T new_value)
        : v{new_value} {}

    [[nodiscard]] SAFE_INLINE constexpr auto value() const -> T { return v; }
};

} // namespace safe

template <typename T>
    requires(safe::Var<T>)
[[nodiscard]] SAFE_INLINE constexpr auto unsafe_cast(auto const &src) {
    return T{safe::unsafe_cast_ferry{src}};
}

template <typename T>
    requires(!safe::Var<T>)
[[nodiscard]] SAFE_INLINE constexpr auto unsafe_cast(auto const &src) {
    return src;
}
