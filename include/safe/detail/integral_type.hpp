#pragma once


#include <safe/detail/fwd.hpp>

#include <type_traits>


namespace safe::detail {
    template<typename T>
    using integral_type =
        var<T, ival<
            std::numeric_limits<T>::lowest(),
            std::numeric_limits<T>::max()>
        >;
}