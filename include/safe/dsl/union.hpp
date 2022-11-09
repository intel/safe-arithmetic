#pragma once

namespace safe::dsl {
    template<typename... Intervals>
    struct union_t {
        using type = union_t;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator||(LhsT, RhsT)
        -> union_t<LhsT, RhsT>
    {
        return {};
    }
}