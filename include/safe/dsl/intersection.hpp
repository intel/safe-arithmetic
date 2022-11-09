#pragma once

namespace safe::dsl {
    template<typename... Intervals>
    struct intersection_t {
        using type = intersection_t;
    };
    
    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator&&(LhsT, RhsT)
        -> intersection_t<LhsT, RhsT>
    {
        return {};
    }
}