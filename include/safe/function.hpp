#pragma once


#include <safe/var.hpp>
#include <safe/detail/invoke.hpp>

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
     * @tparam RetT
     *      The return type of the function object.
     *
     * @param func
     *      The first function to attempt to apply to the arguments.
     *
     * @param remaining_funcs
     *      The remaining functions to attempt to apply to the arguments.
     *
     * @return A function object.
     */
    template<typename RetT>
    [[nodiscard]] inline constexpr auto function(
        auto func,
        auto... remaining_funcs
    ) {
        using namespace boost::mp11;
        
        using func_arg_types =
            detail::function_args_t<decltype(func)>;

        if constexpr (std::is_same_v<void, RetT>) {
            return [=](auto && ... args) -> bool {
                static_assert(
                    mp_size<func_arg_types>::value == sizeof...(args),
                    "The number of arguments passed in must match the args in func.");

                bool const args_satisfy_reqs =
                    detail::check(func_arg_types{}, std::forward<decltype(args)>(args)...);

                if (args_satisfy_reqs) {
                    func(std::forward<decltype(args)>(args)...);
                    return true;

                } else if constexpr (sizeof...(remaining_funcs) > 0) { // check the remaining functions' requirements
                    return function<RetT>(remaining_funcs...)(std::forward<decltype(args)>(args)...);

                } else { // no match, return failure
                    return false;
                }
            };

        } else {
            return [=](auto && ... args) -> RetT {
                if constexpr (sizeof...(remaining_funcs) == 0) {
                    static_assert(
                        mp_size<func_arg_types>::value == 0,
                        "Last function in `safe::function` returns the default value and must not take any arguments.");

                    return func();

                } else {
                    static_assert(
                        mp_size<func_arg_types>::value == sizeof...(args),
                        "The number of arguments passed in must match the args in func.");

                    bool const args_satisfy_reqs =
                        detail::check(func_arg_types{}, std::forward<decltype(args)>(args)...);

                    if (args_satisfy_reqs) {
                        return func(std::forward<decltype(args)>(args)...);

                    } else { // check the remaining functions' requirements
                        return function<RetT>(remaining_funcs...)(std::forward<decltype(args)>(args)...);
                    }
                }
            };
        }
    }
}