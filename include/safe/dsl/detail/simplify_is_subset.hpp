#pragma once

#include <safe/dsl/is_subset.hpp>
#include <safe/dsl/union.hpp>
#include <safe/dsl/intersection.hpp>
#include <safe/dsl/simplify_fwd.hpp>


namespace safe::dsl::detail {
    template<typename... Ts>
    struct all_of {
        using type = all_of;
        constexpr static bool value = (simplify_t<Ts>::value && ...);

        [[nodiscard]] constexpr operator bool() const {
            return value;
        }
    };

    template<typename... Ts>
    struct any_of {
        using type = any_of;
        constexpr static bool value = (simplify_t<Ts>::value || ...);

        [[nodiscard]] constexpr operator bool() const {
            return value;
        }
    };


    template<typename... LhsTs, typename RhsT>
    struct simplify<safe::dsl::is_subset<union_t<LhsTs...>, RhsT>>
        : public all_of<safe::dsl::is_subset<LhsTs, simplify_t<RhsT>>...>
    {};

    template<typename LhsT, typename... RhsTs>
    struct simplify<safe::dsl::is_subset<LhsT, union_t<RhsTs...>>>
        : public any_of<safe::dsl::is_subset<simplify_t<LhsT>, RhsTs>...>
    {};

    template<typename... LhsTs, typename... RhsTs>
    struct simplify<safe::dsl::is_subset<union_t<LhsTs...>, union_t<RhsTs...>>>
        : public all_of<safe::dsl::is_subset<LhsTs, union_t<RhsTs...>>...>
    {};

    template<typename LhsT, typename RhsT>
    struct simplify<
        safe::dsl::is_subset<LhsT, RhsT>,
        std::enable_if_t<
            !is_union_v<LhsT> &&
            !is_union_v<RhsT> &&
            (
                !is_primitive_v<LhsT> ||
                !is_primitive_v<RhsT>
            )
        >
    >
        : public simplify_t<is_subset<simplify_t<LhsT>, simplify_t<RhsT>>>
    {};
}