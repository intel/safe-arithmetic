#pragma once


#include <safe/var.hpp>
#include <safe/detail/function.hpp>

#include <boost/mp11.hpp>

#include <utility>
#include <type_traits>


namespace safe {
    /**
     * @brief Create a safe, optionally piece-wise, function.
     *
     * If the arguments satisfy the first function's requirements, it is called
     * and its return value is returned. If not, the second function is tried,
     * and so on. If none of the arguments work for any of the functions' arguments,
     * the last function is called with no arguments and its return value is
     * returned.
     *
     * If RetT is void, then no default function should be specified and the
     * function object's return value will be bool.
     *
     * @param func
     *      The first function to attempt to apply to the arguments.
     *
     * @param remaining_funcs
     *      The remaining functions to attempt to apply to the arguments.
     *
     * @return A function object.
     */
    template<typename RetT> // FIXME: calculate common return type
    [[nodiscard]] inline constexpr auto match(
        auto func,
        auto... remaining_funcs
    ) {
        using namespace boost::mp11;
        
        using func_arg_types =
            detail::function_args_t<decltype(func)>;

        return [=](auto && ... args) -> RetT {
            if constexpr (sizeof...(remaining_funcs) == 0) {
                static_assert(
                    mp_size<func_arg_types>::value == 0,
                    "Last function in `safe::match` returns the default value and must not take any arguments.");

                return func();

            } else {
                static_assert(
                    mp_size<func_arg_types>::value == sizeof...(args),
                    "The number of arguments passed in must match the args in func.");

                bool const args_satisfy_reqs =
                    detail::check(func_arg_types{}, std::forward<decltype(args)>(args)...);

                if (args_satisfy_reqs) {
                    return func(unsafe_cast_ferry{detail::unwrap_var(std::forward<decltype(args)>(args))}...);

                } else { // check the remaining functions' requirements
                    return match<RetT>(remaining_funcs...)(std::forward<decltype(args)>(args)...);
                }
            }
        };
    }
}