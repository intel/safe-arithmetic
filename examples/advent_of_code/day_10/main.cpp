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



// https://adventofcode.com/2022/day/10

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


struct signal_t {
    int cycle;
    int value;
};

int main() {
    auto const start = now();

    auto input =
        std::ifstream{"input.txt"};

    auto lines =
        r::getlines_view(input);

    auto const mid = now();

    std::map<int, int> signal_values{{-1, 1}, {0, 1}, {1, 1}};


    r::fold_left(lines.begin(), lines.end(), signal_t{1, 1},
        [&](signal_t signal, auto const & line) -> signal_t {
            auto const [noop_result] =
                scn::scan_tuple<>(line, "noop");

            if (noop_result) {
                signal.cycle++;
            }

            auto const [addx_result, addx_amt] =
                scn::scan_tuple<int>(line, "addx {}");

            if (addx_result) {
                signal.cycle += 2;
                signal.value += addx_amt;
            }

            signal_values[signal.cycle] = signal.value;
            fmt::print("{:10d}: {}\n", signal.cycle, signal.value);
            return signal;
        });

    fmt::print("\n\n");

    int part1_signal_str = 0;
    for (int i = 20; i <= 240; i += 40) {
        auto const value = (--signal_values.upper_bound(i))->second;
        fmt::print("> {} = {}\n", i, value);
        part1_signal_str += (i * value);
    }

    fmt::print("part1 answer {}\n", part1_signal_str);

    fmt::print("\n");
    for (int i = 1; i <= 240; i++) {
        auto const value = (--signal_values.upper_bound(i))->second;
        auto const x = ((i - 1) % 40);
        if (std::abs(x - value) < 2) {
            fmt::print("██");
        } else {
            fmt::print("  ");
        }

        if ((i % 40) == 0) {
            fmt::print("\n");
        }
    }
    fmt::print("\n");

    auto const end = now();

    std::cout << "file io time: " << to_nanos(mid - start) << " ns" << std::endl;
    std::cout << "calc time: " << to_nanos(end - mid) << " ns" << std::endl;

    return 0;
}