#pragma once


#include <safe/var.hpp>
#include <safe/constant.hpp>
#include <safe/dsl/simplify.hpp>

#include <type_traits>


namespace safe {
    namespace detail {
        template<auto count>
        inline constexpr auto fold(auto e){
            if constexpr (count > 100) {
                return safe::dsl::detail::simp(fold<count - 100>(e) +
                    e + e + e + e + e + e + e + e + e + e +
                    e + e + e + e + e + e + e + e + e + e +
                    e + e + e + e + e + e + e + e + e + e +
                    e + e + e + e + e + e + e + e + e + e +
                    e + e + e + e + e + e + e + e + e + e +
                    e + e + e + e + e + e + e + e + e + e +
                    e + e + e + e + e + e + e + e + e + e +
                    e + e + e + e + e + e + e + e + e + e +
                    e + e + e + e + e + e + e + e + e + e +
                    e + e + e + e + e + e + e + e + e + e);

            } else if constexpr (count > 10) {
                return safe::dsl::detail::simp(fold<count - 10>(e) +
                    e + e + e + e + e + e + e + e + e + e);

            } else if constexpr (count > 1) {
                return safe::dsl::detail::simp(fold<count - 1>(e) + e);

            } else {
                return e;
            }
        }
    }

    template<size_t max_iter>
    [[nodiscard]] inline constexpr auto accumulate(auto begin, auto end) {
        constexpr decltype((*begin).requirement) req{};
        constexpr auto sum_req = detail::fold<max_iter>(req);
        using ret_num_t = decltype((*begin).unsafe_value());

        auto iter_count = size_t{};
        ret_num_t sum = 0;
        for (auto elem = begin; (elem != end) && (iter_count < max_iter); elem++) {
            sum += (*elem).unsafe_value();
            iter_count++;
        }

        return safe::var<ret_num_t, sum_req>{sum};
    }

    template<size_t max_iter>
    [[nodiscard]] inline constexpr auto accumulate(auto & range) {
        return accumulate<max_iter>(range.begin(), range.end());
    }

    template<size_t max_iter>
    [[nodiscard]] inline constexpr auto accumulate(auto const & range) {
        return accumulate<max_iter>(range.begin(), range.end());
    }

    namespace views {
        template<size_t max_iter>
        struct accumulate_t
        {
            template<typename Rng>
            constexpr auto operator()(Rng&& rng) const
            {
                return safe::accumulate<max_iter>(rng);
            }
        };

        template<size_t max_iter>
        inline constexpr auto accumulate = accumulate_t<max_iter>{};
    }
}