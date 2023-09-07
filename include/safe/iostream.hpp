#pragma once

#include <iostream>

#include <safe.hpp>

auto operator<<(std::ostream &os, safe::Var auto var) -> std::ostream & {
    return os << var.unsafe_value();
}
