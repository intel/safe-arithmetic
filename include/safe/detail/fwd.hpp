#pragma once

#ifndef SAFE_INLINE
#define SAFE_INLINE inline
#endif

namespace safe {
template <auto Constraint, typename T> struct constrained_number;

template <typename T> constexpr bool is_var_v = false;

template <typename T, auto Constraint>
constexpr bool is_var_v<constrained_number<Constraint, T>> = true;

template <typename T>
concept any_constrained = is_var_v<T>;

[[nodiscard]] constexpr inline auto value(auto value);

namespace detail {
template <typename U, U value>
[[nodiscard]] constexpr inline auto make_constant();
}

template <typename T> struct constraint_cast_ferry {
  private:
    T v;

  public:
    SAFE_INLINE constexpr explicit(true) constraint_cast_ferry(T new_value)
        : v{new_value} {}

    [[nodiscard]] SAFE_INLINE constexpr auto value() const -> T { return v; }
};

} // namespace safe

template <typename T>
    requires(safe::any_constrained<T>)
[[nodiscard]] constexpr auto constraint_cast(auto const &src) {
    return T{safe::constraint_cast_ferry{src}};
}

template <typename T>
    requires(!safe::any_constrained<T>)
[[nodiscard]] constexpr auto constraint_cast(auto const &src) {
    return src;
}
