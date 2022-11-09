#include <safe.hpp>


void test() {
    safe::ival<0u, 1u> << safe::ival<0u, 32u>;
}
