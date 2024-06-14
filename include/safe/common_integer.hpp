#pragma once 

#include <concepts>
#include <type_traits>

namespace safe {
template <typename L, typename R> struct common_integer;


template <std::integral L, std::integral R>
struct common_integer<L, R> {
    using type = std::common_type_t<L, R>;
};


template <typename L, typename R>
using common_integer_t = typename common_integer<std::remove_cvref_t<L>,
                                                 std::remove_cvref_t<R>>::type;

}