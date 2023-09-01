#pragma once

#include <safe/dsl.hpp>
#include <safe/var.hpp>

namespace safe::detail {
template <typename U, U value>
[[nodiscard]] constexpr inline auto make_constant() {
    return unsafe_cast<var<U, ival<value, value>>>(value);
}
} // namespace safe::detail