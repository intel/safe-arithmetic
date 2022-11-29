#pragma once

#include <safe/var.hpp>

#include <boost/mp11.hpp>

namespace safe {

    namespace detail {
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

    template<typename Callable, typename... ArgTs>
    constexpr bool invoke(
        Callable c,
        ArgTs... args
    ) {
        using var_list = detail::function_args_t<Callable>;
        using input_args = boost::mp11::mp_list<ArgTs...>;

        static_assert(
            boost::mp11::mp_size<var_list>::value ==
            boost::mp11::mp_size<input_args>::value);

        bool const is_safe =
            detail::check(var_list{}, args...);

        if (is_safe) {
            c(args...);
            return true;
        } else {
            return false;
        }
    }
}