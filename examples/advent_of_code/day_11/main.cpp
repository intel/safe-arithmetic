#include <safe.hpp>
#include <safe/iostream.hpp>

#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
#include <range/v3/algorithm.hpp>

#include <scn/scn.h>
#include <scn/tuple_return.h>

#include <fmt/core.h>

#include <iostream>
#include <fstream>
#include <string_view>
#include <chrono>
#include <charconv>
#include <bit>
#include <map>



// https://adventofcode.com/2022/day/11

using namespace safe::literals;
using namespace safe::int_types;
using namespace safe::interval_types;

namespace r = ranges;
namespace rv = ranges::views;

constexpr auto & now =
        std::chrono::steady_clock::now;

constexpr auto to_micros = [](auto val) {
    return std::chrono::duration_cast<std::chrono::microseconds>(val).count();
};

constexpr auto to_nanos = [](auto val) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(val).count();
};


using op_fn_ptr = long(*)(long);

struct monkey_t {
    std::vector<long> items{};
    op_fn_ptr op{};
    long div_test{};
    long true_dst{};
    long false_dst{};
    long inspections{};
};

int main() {
    auto const start = now();

    std::array<monkey_t, 8> monkeys{{
        {
            {98, 70, 75, 80, 84, 89, 55, 98},
            [](long old) -> long {return old * 2;},
            11, 1, 4
        },
        {
            {59},
            [](long old) -> long {return old * old;},
            19, 7, 3
        },
        {
            {77, 95, 54, 65, 89},
            [](long old) -> long {return old + 6;},
            7, 0, 5
        },
        {
            {71, 64, 75},
            [](long old) -> long {return old + 2;},
            17, 6, 2
        },
        {
            {74, 55, 87, 98},
            [](long old) -> long {return old * 11;},
            3, 1, 7
        },
        {
            {90, 98, 85, 52, 91, 60},
            [](long old) -> long {return old + 7;},
            5, 0, 4
        },
        {
            {99, 51},
            [](long old) -> long {return old + 1;},
            13, 5, 2
        },
        {
            {98, 94, 59, 76, 51, 65, 75},
            [](long old) -> long {return old + 5;},
            2, 3, 6
        }
    }};


//    std::array<monkey_t, 4> monkeys{{
//        {
//            {79, 98},
//            [](long old) -> long {return old * 19;},
//            23, 2, 3
//        },
//        {
//            {54, 65, 75, 74},
//            [](long old) -> long {return old + 6;},
//            19, 2, 0
//        },
//        {
//            {79, 60, 97},
//            [](long old) -> long {return old * old;},
//            13, 1, 3
//        },
//        {
//            {74},
//            [](long old) -> long {return old + 3;},
//            17, 0, 1
//        }
//    }};

//    auto input =
//        std::ifstream{"input.txt"};
//
//    auto lines =
//        r::getlines_view(input);

    auto const mid = now();

    long mod_base = 1;
    for (auto & monkey : monkeys) {
        mod_base *= monkey.div_test;
    }

    fmt::print("mod_base = {}\n", mod_base);

    for (long i = 0; i < 10000; i++) {
        for (auto & monkey : monkeys) {
            for (auto item : monkey.items) {
                monkey.inspections++;
                auto const new_worry = monkey.op(item) % mod_base;
                if ((new_worry % monkey.div_test) == 0) {
                    monkeys[monkey.true_dst].items.push_back(new_worry);
                } else {
                    monkeys[monkey.false_dst].items.push_back(new_worry);
                }
            }
            monkey.items.clear();
        }
    }

    for (auto const & monkey : monkeys) {
        fmt::print("inspections: {}\n", monkey.inspections);
    }

    fmt::print("part1 answer {}\n", 0);

    auto const end = now();

    std::cout << "file io time: " << to_nanos(mid - start) << " ns" << std::endl;
    std::cout << "calc time: " << to_nanos(end - mid) << " ns" << std::endl;

    return 0;
}