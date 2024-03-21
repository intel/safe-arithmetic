#pragma once

#include <safe/constrained_number.hpp>

#include <boost/mp11.hpp>

namespace safe::detail {
using namespace boost::mp11;

template <typename ArgT, typename InputT> struct runtime {
    [[nodiscard]] constexpr static auto check(InputT) -> bool { return true; }
};

template <any_constrained VarT, typename InputT> struct runtime<VarT, InputT> {
    [[nodiscard]] constexpr static auto check(InputT const &input) -> bool {
        return VarT::constraint.check(input);
    }
};

template <any_constrained VarT, any_constrained InputVarT> struct runtime<VarT, InputVarT> {
    [[nodiscard]] constexpr static auto check(InputVarT const &input) -> bool {
        if constexpr (VarT::constraint >= InputVarT::constraint) {
            return true;
        } else {
            return VarT::constraint.check(input.unsafe_value());
        };
    }
};

template <typename VarT, typename ValueT>
[[nodiscard]] constexpr auto check(ValueT value) -> bool {
    return detail::runtime<VarT, ValueT>::check(value);
}

template <typename T, typename ReturnT, typename... ArgTs>
auto ret_helper(ReturnT (T::*)(ArgTs...)) -> ReturnT;

template <typename T, typename ReturnT, typename... ArgTs>
auto ret_helper(ReturnT (T::*)(ArgTs...) const) -> ReturnT;

template <typename T, typename ReturnT, typename... ArgTs>
auto args_helper(ReturnT (T::*)(ArgTs...)) -> mp_list<ArgTs...>;

template <typename T, typename ReturnT, typename... ArgTs>
auto args_helper(ReturnT (T::*)(ArgTs...) const) -> mp_list<ArgTs...>;

template <typename F> struct function_info {
    using ret = decltype(ret_helper(&F::operator()));
    using args = decltype(args_helper(&F::operator()));
};

template <typename ReturnT, typename... ArgTs>
struct function_info<ReturnT(ArgTs...)> {
    using ret = ReturnT;
    using args = mp_list<ArgTs...>;
};

template <typename ReturnT, typename... ArgTs>
struct function_info<ReturnT (*)(ArgTs...)> {
    using ret = ReturnT;
    using args = mp_list<ArgTs...>;
};

template <typename F> using function_args_t = typename function_info<F>::args;

template <typename F> using function_ret_t = typename function_info<F>::ret;

template <typename... ContractTs, typename... ArgTs>
constexpr auto check(mp_list<ContractTs...>, ArgTs... args) -> bool {
    return (check<ContractTs, ArgTs>(args) && ...);
}

template <typename T>
[[nodiscard]] constexpr auto unwrap_var(T &&v) -> decltype(auto) {
    if constexpr (any_constrained<std::remove_cvref_t<T>>) {
        return v.unsafe_value();
    } else {
        return std::forward<decltype(v)>(v);
    }
}
} // namespace safe::detail
