#include <safe.hpp>

using namespace safe::literals;

void test() {
    safe::ival_s32<0, 10> test = 0_cn;
    test = 20_cn;
}
