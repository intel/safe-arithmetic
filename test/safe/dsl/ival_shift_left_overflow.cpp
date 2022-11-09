#include <safe.hpp>


void test() {
    safe::ival<0, 1> << safe::ival<0, 31>;
}
