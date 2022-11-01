#include <safe.hpp>

using namespace safe::literals;
using safe::ival;

void test() {
    safe::var<int, ival<1, 30>> a = 1_s32;
    safe::var<int, ival<0, 11> || ival<21, 29>> b = 0_s32;
    a = b;
}
