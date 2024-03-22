#pragma once

#include <safe/constant.hpp>
#include <safe/detail/concepts.hpp>
#include <safe/dsl/eval.hpp>
#include <safe/constrained_number.hpp>

#include <type_traits>
#include <utility>

namespace safe {
namespace detail {
template <auto count> inline consteval auto fold(auto e, auto op) {
    if constexpr (count > 100) {
                return safe::dsl::detail::simp(
                    op(op(op(op(op(op(op(op(op(op(
                    op(op(op(op(op(op(op(op(op(op(
                    op(op(op(op(op(op(op(op(op(op(
                    op(op(op(op(op(op(op(op(op(op(
                    op(op(op(op(op(op(op(op(op(op(
                    op(op(op(op(op(op(op(op(op(op(
                    op(op(op(op(op(op(op(op(op(op(
                    op(op(op(op(op(op(op(op(op(op(
                    op(op(op(op(op(op(op(op(op(op(
                    op(op(op(op(op(op(op(op(op(op(fold<count - 10>(e, op),
                    e), e), e), e), e), e), e), e), e), e),
                    e), e), e), e), e), e), e), e), e), e),
                    e), e), e), e), e), e), e), e), e), e),
                    e), e), e), e), e), e), e), e), e), e),
                    e), e), e), e), e), e), e), e), e), e),
                    e), e), e), e), e), e), e), e), e), e),
                    e), e), e), e), e), e), e), e), e), e),
                    e), e), e), e), e), e), e), e), e), e),
                    e), e), e), e), e), e), e), e), e), e),
                    e), e), e), e), e), e), e), e), e), e)
                );

    } else if constexpr (count > 10) {
        return safe::dsl::detail::simp(op(
            op(op(op(op(op(op(op(op(op(fold<count - 10>(e, op), e), e), e), e),
                           e),
                        e),
                     e),
                  e),
               e),
            e));

    } else if constexpr (count > 1) {
        return safe::dsl::detail::simp(op(fold<count - 1>(e, op), e));

    } else {
        return e;
    }
}

constexpr inline auto plus_op = [](auto a, auto b) { return a + b; };
} // namespace detail

template <size_t max_iter>
[[nodiscard]] constexpr inline auto accumulate(detail::iter_like auto first,
                                               auto last, auto init, auto op) {
    constexpr auto req = decltype((*first).constraint){};
    constexpr auto sum_req = detail::fold<max_iter>(req, op);

    using ret_num_t = decltype((*first).raw_value());

    auto iter_count = size_t{};
    auto sum = init;
    while ((first != last) && (iter_count < max_iter)) {
        sum = op(sum, (*first).raw_value());
        first++;
        iter_count++;
    }

    return constraint_cast<sum_req, ret_num_t>(sum);
}

template <size_t max_iter>
[[nodiscard]] constexpr inline auto accumulate(detail::iter_like auto first,
                                               auto last, auto init) {
    return accumulate<max_iter>(first, last, init, detail::plus_op);
}

template <size_t max_iter>
[[nodiscard]] constexpr inline auto accumulate(detail::range_like auto &range,
                                               auto init, auto op) {
    return accumulate<max_iter>(range.begin(), range.end(), init, op);
}

template <size_t max_iter>
[[nodiscard]] constexpr inline auto accumulate(detail::range_like auto &range,
                                               auto init) {
    return accumulate<max_iter>(range.begin(), range.end(), init,
                                detail::plus_op);
}

template <size_t max_iter>
[[nodiscard]] constexpr inline auto
accumulate(detail::range_like auto const &range, auto init, auto op) {
    return accumulate<max_iter>(range.begin(), range.end(), init, op);
}

template <size_t max_iter>
[[nodiscard]] constexpr inline auto accumulate(auto const &range, auto init) {
    return accumulate<max_iter>(range.begin(), range.end(), init,
                                detail::plus_op);
}
} // namespace safe