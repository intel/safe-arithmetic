#pragma once

#include <safe/dsl/intersection.hpp>
#include <safe/dsl/constrain_interval.hpp>
#include <safe/dsl/constrain_mask.hpp>
#include <safe/dsl/primitive.hpp>
#include <safe/dsl/union.hpp>

#include <boost/mp11.hpp>

namespace safe::dsl::detail {
using namespace boost::mp11;

template <typename T, typename Enable = void> struct eval {
    using type = typename T::type;
    constexpr static type value{};
};

template <typename T> using eval_t = typename eval<T>::type;

template <typename T> constexpr auto eval_v = eval<T>::value;

template <typename T> [[nodiscard]] constexpr auto simp(T) {
    return eval_t<T>{};
}

template <typename T>
struct is_union : public std::integral_constant<bool, false> {};

template <typename... Ts>
struct is_union<union_t<Ts...>> : public std::integral_constant<bool, true> {};

template <typename T> constexpr bool is_union_v = is_union<T>{};
} // namespace safe::dsl::detail