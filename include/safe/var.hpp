#pragma once

#include <safe/contract.hpp>

namespace safe {
    // FIXME: look at requirements and T and set some default requirements
    template<typename T, typename... Requirements>
    using var = contract<T, Requirements...>;
}