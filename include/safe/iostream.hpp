#pragma once

#include <iostream>

#include <safe.hpp>

auto operator<<(std::ostream &os, safe::any_constrained auto constrained_number) -> std::ostream & {
    return os << constrained_number.raw_value();
}
