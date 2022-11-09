#pragma once

#include <safe/dsl/union.hpp>
#include <safe/dsl/intersection.hpp>

#include <boost/mp11.hpp>

namespace safe::dsl::detail {
    using namespace boost::mp11;
    
    template<typename T, typename Enable = void>
    struct simplify {
        using type = typename T::type;
        constexpr static type value{};
    };

    template<typename T>
    using simplify_t = typename simplify<T>::type;

    template<typename T>
    constexpr auto simplify_v = simplify<T>::value;

    template<typename T>
    [[nodiscard]] constexpr auto simp(T) {
        return simplify_t<T>{};
    }
}