#pragma once

#include <safe/detail/pure.hpp>
#include <safe/dsl/fwd.hpp>

namespace safe::dsl {
template <typename... Intervals> struct union_t : public detail::set_op {
    using type = union_t;

    template <typename T>
    [[nodiscard]] SAFE_PURE constexpr static auto check(T value) -> bool {
        return (Intervals::check(value) || ...);
    }
};

template <any_constraint LhsT, any_constraint RhsT>
[[nodiscard]] constexpr auto operator||(LhsT, RhsT) -> union_t<LhsT, RhsT> {
    return {};
}
} // namespace safe::dsl
