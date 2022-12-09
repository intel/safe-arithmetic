#pragma once

#include <safe/dsl/ival.hpp>
#include <safe/detail/fwd.hpp>

#include <type_traits>
#include <limits>


namespace safe::detail {
    template<typename T>
    using integral_type =
        var<T, safe::dsl::ival<
            std::numeric_limits<T>::lowest(),
            std::numeric_limits<T>::max()>
        >;
}