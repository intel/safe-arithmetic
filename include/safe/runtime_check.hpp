#pragma once

#include <safe/var.hpp>
#include <safe/dsl.hpp>

namespace safe {
    namespace detail {
        template<
            typename VarT,
            typename ValueT>
        struct runtime {
            [[nodiscard]] constexpr static bool check(ValueT value) {
                return VarT::requirement.check(value);
            }
        };

        template<
            typename VarT,
            typename ValueValueT,
            auto ValueRequirement>
        struct runtime<
            VarT,
            var<ValueValueT, ValueRequirement>
        > {
            [[nodiscard]] constexpr static bool check(var<ValueValueT, ValueRequirement>) {
                return VarT::requirement >= ValueRequirement;
            }
        };
    }

    template<
        typename VarT,
        typename ValueT>
    [[nodiscard]] constexpr bool check(ValueT value) {
        return detail::runtime<VarT, ValueT>::check(value);
    }
}

