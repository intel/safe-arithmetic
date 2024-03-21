#include <safe.hpp>

using namespace safe::literals;
using safe::constrain_interval;

void test() {
    safe::constrained_number<constrain_interval<0, 10> || constrain_interval<20, 30>, int> a = 0_s32;
    safe::constrained_number<constrain_interval<1, 11>, int> b = 11_s32;
    a = b;
}
