#pragma once

#include <safe/detail/fwd.hpp>

namespace safe {
namespace detail {
template <typename T> struct constraint_cast_ferry {
  private:
    T v;

  public:
    SAFE_INLINE constexpr explicit(true) constraint_cast_ferry(T new_value)
        : v{new_value} {}

    [[nodiscard]] SAFE_INLINE constexpr auto value() const -> T { return v; }
};    
} // namespace detail

template <typename T>
    requires(any_constrained<T>)
[[nodiscard]] constexpr auto constraint_cast(auto const &src) {
    return T{detail::constraint_cast_ferry{src}};
}

template <typename T>
    requires(!any_constrained<T>)
[[nodiscard]] constexpr auto constraint_cast(auto const &src) {
    return src;
}
} // namespace safe