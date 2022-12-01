#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/simplify_fwd.hpp>
#include <safe/dsl/detail/checked.hpp>

namespace safe::dsl {
    template<typename T, typename U>
    struct minus : public binary_op {};

    template<
        Interval LhsT,
        Interval RhsT>
    struct minus<LhsT, RhsT>
        : public binary_op
    {
        using type = ival_t<
            detail::c_<LhsT::min> - detail::c_<RhsT::max>,
            detail::c_<LhsT::max> - detail::c_<RhsT::min>
        >;
    };

    template<
        Mask LhsT,
        Mask RhsT>
    struct minus<LhsT, RhsT>
        : public binary_op
    {
        constexpr static auto value = LhsT::value - RhsT::value;
        using type = mask_t<value.var_bits(), value.const_bits()>;
    };

    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator-(LhsT, RhsT)
        -> minus<LhsT, RhsT>
    {
        return {};
    }
}