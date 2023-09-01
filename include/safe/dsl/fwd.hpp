#pragma once

namespace safe::dsl::detail {
struct binary_op {};
struct unary_op {};
struct primitive {};
struct set_op {};

// FIXME: make the min/max template vars take an arbitrary num of args
template <auto lhs, auto rhs>
constexpr auto min2 = [] {
    if constexpr (lhs <= rhs) {
        return lhs;
    } else {
        return rhs;
    }
}();

template <auto a, auto b, auto c, auto d>
constexpr auto min = [] { return min2<min2<a, b>, min2<c, d>>; }();

template <auto lhs, auto rhs>
constexpr auto max2 = [] {
    if constexpr (lhs > rhs) {
        return lhs;
    } else {
        return rhs;
    }
}();

template <auto a, auto b, auto c, auto d>
constexpr auto max = [] { return max2<max2<a, b>, max2<c, d>>; }();
} // namespace safe::dsl::detail

namespace safe::dsl {
template <typename T>
concept Operand = std::is_base_of_v<detail::binary_op, T> ||
                  std::is_base_of_v<detail::unary_op, T> ||
                  std::is_base_of_v<detail::primitive, T> ||
                  std::is_base_of_v<detail::set_op, T>;

}
