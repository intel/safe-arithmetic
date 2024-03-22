#pragma once

#include <safe/dsl/fwd.hpp>

namespace safe::dsl {
template <typename... Intervals> struct intersection_t : public detail::set_op {
    using type = intersection_t;
};

template <any_constraint LhsT, any_constraint RhsT>
[[nodiscard]] constexpr auto operator&&(LhsT, RhsT)
    -> intersection_t<LhsT, RhsT> {
    return {};
}
} // namespace safe::dsl