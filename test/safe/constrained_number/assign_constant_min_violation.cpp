#include <safe.hpp>

using namespace safe::literals;

void test() {
    safe::ival_s32<0, 10> test = 0_s32;
    test = -1_s32;
}
