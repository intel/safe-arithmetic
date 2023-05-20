#include <safe.hpp>
//#include <safe/iostream.hpp>
//
//#include <range/v3/view.hpp>
//#include <range/v3/action.hpp>
//#include <range/v3/algorithm.hpp>
//
//#include <scn/scn.h>
//#include <scn/tuple_return.h>
//
//#include <fmt/core.h>
//
//#include <iostream>
//#include <fstream>
//#include <string_view>
//#include <chrono>
//#include <charconv>
//#include <bit>
//#include <map>



// https://adventofcode.com/2022/day/12

using namespace safe::literals;
using namespace safe::int_types;
using namespace safe::interval_types;

using elev_t = ival_s8<0, 27>;

volatile int input = 0;
volatile int output = 0;

constexpr auto to_elev =
    safe::function<elev_t>(
        [](ival_s32<'a', 'z'> c){return c - s32_<'a'>;},
        [](){return 0_u32;}
    );

static int unsafe_to_elev(int c) {
    if (c >= 'a' and c <= 'z') {
        return c - 'a';
    } else {
        return 0;
    }
}

int main() {
    output = to_elev(input).unsafe_value();
//    output = unsafe_to_elev(input);
    return 0;
}