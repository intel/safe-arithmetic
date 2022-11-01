#include <safe.hpp>

using namespace safe::literals;

void test() {
    safe::ival_s32<0, 10> test = 11_s32;
}
