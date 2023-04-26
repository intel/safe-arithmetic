#pragma once


#include <safe/var.hpp>
#include <safe/constant.hpp>
#include <safe/dsl/simplify.hpp>

#include <safe/detail/concepts.hpp>

#include <type_traits>
#include <utility>


namespace safe {
    namespace detail {
        template<auto count>
        inline consteval auto fold(auto e, auto op){
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
                return safe::dsl::detail::simp(
                    op(op(op(op(op(op(op(op(op(op(fold<count - 10>(e, op), e), e), e), e), e), e), e), e), e), e)
                );

            } else if constexpr (count > 1) {
                return safe::dsl::detail::simp(op(fold<count - 1>(e, op), e));

            } else {
                return e;
            }
        }

        inline constexpr auto plus_op = [](auto a, auto b){return a + b;};
    }


    template<size_t max_iter>
    [[nodiscard]] inline constexpr auto accumulate(
        detail::iter_like auto first,
        auto last,
        auto op
    ) {
        constexpr auto req = decltype((*first).requirement){};
        constexpr auto sum_req = detail::fold<max_iter>(req, op);

        using ret_num_t = decltype((*first).unsafe_value());

        auto iter_count = size_t{};
        auto sum = ret_num_t{};
        while ((first != last) && (iter_count < max_iter)) {
            sum = op(sum, (*first).unsafe_value());
            first++;
            iter_count++;
        }

        return unsafe_cast<var<ret_num_t, sum_req>>(sum);
    }

    template<size_t max_iter>
    [[nodiscard]] inline constexpr auto accumulate(
        detail::iter_like auto first,
        auto last
    ) {
        return accumulate<max_iter>(first, last, detail::plus_op);
    }

    template<size_t max_iter>
    [[nodiscard]] inline constexpr auto accumulate(
        detail::range_like auto & range,
        auto op
    ) {
        return accumulate<max_iter>(range.begin(), range.end(), op);
    }

    template<size_t max_iter>
    [[nodiscard]] inline constexpr auto accumulate(
        detail::range_like auto & range
    ) {
        return accumulate<max_iter>(range.begin(), range.end(), detail::plus_op);
    }

    template<size_t max_iter>
    [[nodiscard]] inline constexpr auto accumulate(
        detail::range_like auto const & range,
        auto op
    ) {
        return accumulate<max_iter>(range.begin(), range.end(), op);
    }

    template<size_t max_iter>
    [[nodiscard]] inline constexpr auto accumulate(auto const & range) {
        return accumulate<max_iter>(range.begin(), range.end(), detail::plus_op);
    }
}