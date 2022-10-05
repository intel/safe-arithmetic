#include <safe/contract.hpp>
#include <safe/array.hpp>

void test() {
    safe::array<int, 10> my_data{};
    int value_one = my_data[safe::constant<size_t, 10>];
}
