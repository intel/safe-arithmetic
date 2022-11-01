#include <safe.hpp>

using namespace safe::literals;
using safe::ival;

void test() {
    safe::var<int, ival<0, 10> || ival<20, 30>> a = 0_s32;
    safe::var<int, ival<1, 11>> b = 11_s32;
    a = b;
}
