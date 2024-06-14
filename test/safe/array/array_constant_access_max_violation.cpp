#include <safe/array.hpp>
#include <safe/int.hpp>
#include <safe/constrained_number.hpp>

using namespace safe::literals;

void test() {
    safe::array<int, 10> my_data{};
    int value_one = my_data[10_cn];
}
