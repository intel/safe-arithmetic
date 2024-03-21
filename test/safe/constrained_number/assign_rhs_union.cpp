#include <safe.hpp>

using namespace safe::literals;
using safe::constrain_interval;

void test() {
    safe::constrained_number<constrain_interval<1, 30>, int> a = 1_s32;
    safe::constrained_number<constrain_interval<0, 11> || constrain_interval<21, 29>, int> b = 0_s32;
    a = b;
}
