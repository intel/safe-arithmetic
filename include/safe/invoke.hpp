#pragma once

#include <safe/var.hpp>
#include <safe/detail/invoke.hpp>

#include <boost/mp11.hpp>

namespace safe {
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