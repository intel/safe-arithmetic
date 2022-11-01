#include <safe/var.hpp>
#include <safe/array.hpp>
#include <safe/int.hpp>

using namespace safe::literals;

void test() {
    safe::array<int, 10> my_data{};
    int value_one = my_data[10_u64];
}
