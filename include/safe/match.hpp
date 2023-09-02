#pragma once

#include <safe/detail/function.hpp>
#include <safe/var.hpp>

#include <boost/mp11.hpp>

#include <type_traits>
#include <utility>

namespace safe {
namespace detail {
template <typename F, typename... Fs> struct common_ret {
    using type = function_ret_t<F>;
};

template <typename F, typename... Fs>
    requires(Var<function_ret_t<F>> && ... && Var<function_ret_t<Fs>>)
struct common_ret<F, Fs...> {
    constexpr static auto ret_requirement =
        (function_ret_t<F>::requirement || ... ||
         function_ret_t<Fs>::requirement);

    using value_type = typename function_ret_t<F>::value_type;

    using type = var<value_type, ret_requirement>;
};

template <typename... Fs> using common_ret_t = typename common_ret<Fs...>::type;
} // namespace detail

/**
 * @brief Create a safe, optionally piece-wise, function.
 *
 * If the arguments satisfy the first function's requirements, it is called
 * and its return value is returned. If not, the second function is tried,
 * and so on. If none of the arguments work for any of the functions' arguments,
 * the last function is called with no arguments and its return value is
 * returned.
 *
 * @param func
 *      The first function to attempt to apply to the arguments.
 *
 * @param remaining_funcs
 *      The remaining functions to attempt to apply to the arguments.
 *
 * @return A function object.
 */
template <typename F, typename... Fs>
[[nodiscard]] constexpr inline auto match(F func, Fs... remaining_funcs) {
    using namespace boost::mp11;

    using func_arg_types = detail::function_args_t<F>;

    using ret_t = detail::common_ret_t<F, Fs...>;

    return [=](auto &&...args) -> ret_t {
        if constexpr (sizeof...(remaining_funcs) == 0) {
            static_assert(mp_size<func_arg_types>::value == 0,
                          "Last function in `safe::match` returns the default "
                          "value and must not take any arguments.");

            return func();

        } else {
            static_assert(mp_size<func_arg_types>::value == sizeof...(args),
                          "The number of arguments passed in must match the "
                          "args in func.");

            bool const args_satisfy_reqs = detail::check(
                func_arg_types{}, std::forward<decltype(args)>(args)...);

            if (args_satisfy_reqs) {
                return func(unsafe_cast_ferry{
                    detail::unwrap_var(std::forward<decltype(args)>(args))}...);

            } // check the remaining functions' requirements
            return match(remaining_funcs...)(
                std::forward<decltype(args)>(args)...);
        }
    };
}
} // namespace safe
