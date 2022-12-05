#pragma once

#include <safe.hpp>

#include <iostream>

std::ostream& operator<<(std::ostream& os, safe::Var auto var) {
    return os << var.unsafe_value();
}