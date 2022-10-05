#include <safe.hpp>

void test() {
    safe::var<int, safe::interval<0, 10>> test = safe::constant<int, 0>;
    test = safe::constant<int, -1>;
}
