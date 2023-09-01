#pragma once

#include <safe/detail/fwd.hpp>
#include <safe/dsl/ival.hpp>

#include <limits>
#include <type_traits>

namespace safe::detail {
template <typename T>
using integral_type = var<T, safe::dsl::ival<std::numeric_limits<T>::lowest(),
                                             std::numeric_limits<T>::max()>>;
}