#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/dsl/mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/fwd.hpp>
#include <safe/checked.hpp>

namespace safe::dsl {
    template<typename T, typename U>
    struct add : public binary_op {};

    template<
        Interval LhsT,
        Interval RhsT>
    struct add<LhsT, RhsT>
        : public binary_op
    {
        using type = ival_t<
            c_<LhsT::min> + c_<RhsT::min>,
            c_<LhsT::max> + c_<RhsT::max>
        >;
    };

    template<
        Mask LhsT,
        Mask RhsT>
    struct add<LhsT, RhsT>
        : public binary_op
    {
        constexpr static auto value = LhsT::value + RhsT::value;
        using type = mask_t<value.var_bits(), value.const_bits()>;
    };


    template<
        typename LhsT,
        typename RhsT>
    [[nodiscard]] constexpr auto operator+(LhsT, RhsT)
        -> add<LhsT, RhsT>
    {
        return {};
    }
}