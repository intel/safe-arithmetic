#pragma once

#include <safe/contract.hpp>

#include <boost/mp11.hpp>

namespace safe {
    using namespace boost::mp11;

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
        typename ContractT,
        typename ArgT>
    constexpr bool check_arg(
        ArgT arg
    ) {
        return ContractT::check(arg);
    }

    template<
        typename... ContractTs,
        typename... ArgTs>
    constexpr bool check(
        mp_list<ContractTs...>,
        ArgTs... args
    ) {
        return (check_arg<ContractTs>(args) && ...);
    }

    template<typename Callable, typename... ArgTs>
    constexpr bool invoke(
        Callable c,
        ArgTs... args
    ) {
        using contract_list = function_args_t<Callable>;
        using input_args = mp_list<ArgTs...>;

        static_assert(
            mp_size<contract_list>::value ==
            mp_size<input_args>::value);

        bool const is_safe =
            check(contract_list{}, args...);

        if (is_safe) {
            c(args...);
            return true;
        } else {
            return false;
        }
    }
}