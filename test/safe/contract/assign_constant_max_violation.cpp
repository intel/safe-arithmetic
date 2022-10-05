#include <safe.hpp>


void test() {
    safe::var<int, safe::interval<0, 10>> test = safe::clamp<int, 0, 10>(0);
    test = safe::constant<int, 20>;
}
