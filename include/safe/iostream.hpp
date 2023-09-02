#pragma once

#include <iostream>
#include <safe.hpp>

std::ostream &operator<<(std::ostream &os, safe::Var auto var) {
    return os << var.unsafe_value();
}