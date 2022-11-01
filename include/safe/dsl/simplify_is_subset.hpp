#pragma once

#include <safe/dsl/is_subset.hpp>
#include <safe/dsl/union.hpp>
#include <safe/dsl/intersection.hpp>
#include <safe/dsl/simplify_fwd.hpp>


namespace safe_dsl::detail {
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
    struct simplify<safe_dsl::is_subset<union_t<LhsTs...>, RhsT>>
        : public all_of<safe_dsl::is_subset<LhsTs, simplify_t<RhsT>>...>
    {};

    template<typename LhsT, typename... RhsTs>
    struct simplify<safe_dsl::is_subset<LhsT, union_t<RhsTs...>>>
        : public any_of<safe_dsl::is_subset<simplify_t<LhsT>, RhsTs>...>
    {};

    template<typename... LhsTs, typename... RhsTs>
    struct simplify<safe_dsl::is_subset<union_t<LhsTs...>, union_t<RhsTs...>>>
        : public all_of<safe_dsl::is_subset<LhsTs, union_t<RhsTs...>>...>
    {};
}