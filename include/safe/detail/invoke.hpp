#pragma once

#include <safe/var.hpp>

#include <boost/mp11.hpp>

namespace safe::detail {
    using namespace boost::mp11;

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

    template<
        typename VarT,
        typename ValueT>
    [[nodiscard]] constexpr bool check(ValueT value) {
        return detail::runtime<VarT, ValueT>::check(value);
    }


    template<typename F>
    struct function_args;

    template<typename ReturnT, typename... ArgTs>
    struct function_args<ReturnT(ArgTs...)> {
        using type = mp_list<ArgTs...>;
    };

    template<typename ReturnT, typename... ArgTs>
    struct function_args<ReturnT(*)(ArgTs...)> {
        using type = mp_list<ArgTs...>;
    };

    template<typename F>
    using function_args_t = typename function_args<F>::type;

    template<
        typename... ContractTs,
        typename... ArgTs>
    constexpr bool check(
        mp_list<ContractTs...>,
        ArgTs... args
    ) {
        return (check<ContractTs, ArgTs>(args) && ...);
    }
}