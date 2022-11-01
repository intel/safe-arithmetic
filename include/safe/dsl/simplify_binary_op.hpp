#pragma once

#include <safe/dsl/union.hpp>
#include <safe/dsl/intersection.hpp>
#include <safe/dsl/simplify_fwd.hpp>

#include <type_traits>

namespace safe_dsl::detail {
    template<typename T>
    using enable_for_binary_op_t = typename std::enable_if_t<std::is_base_of_v<binary_op, T>>;

    template<
        template<typename, typename> typename op,
        typename... LhsTs,
        typename RhsT>
    struct simplify<
        op<union_t<LhsTs...>, RhsT>,
        enable_for_binary_op_t<op<union_t<LhsTs...>, RhsT>>
    > {
        using type = simplify_t<union_t<op<simplify_t<LhsTs>, simplify_t<RhsT>>...>>;
    };

    template<template<typename, typename> typename op, typename LhsT, typename... RhsTs>
    struct simplify<
        op<LhsT, union_t<RhsTs...>>,
        enable_for_binary_op_t<op<LhsT, union_t<RhsTs...>>>
    > {
        using type = simplify_t<union_t<op<simplify_t<LhsT>, simplify_t<RhsTs>>...>>;
    };

    template<template<typename, typename> typename op, typename... LhsTs, typename... RhsTs>
    struct simplify<
        op<union_t<LhsTs...>, union_t<RhsTs...>>,
        enable_for_binary_op_t<op<union_t<LhsTs...>, union_t<RhsTs...>>>
    > {
        using type = simplify_t<union_t<simplify_t<op<simplify_t<LhsTs>, union_t<simplify_t<RhsTs>...>>>...>>;
    };
}