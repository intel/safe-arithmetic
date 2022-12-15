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
#include <set>



// https://adventofcode.com/2022/day/9

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

using pos_t = std::pair<int, int>;

[[nodiscard]] constexpr auto operator+(pos_t lhs, pos_t rhs) -> pos_t {
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}

[[nodiscard]] constexpr auto operator-(pos_t lhs, pos_t rhs) -> pos_t {
    return {lhs.first - rhs.first, lhs.second - rhs.second};
}

[[nodiscard]] constexpr auto abs(pos_t v) -> pos_t {
    return {std::abs(v.first), std::abs(v.second)};
}

[[nodiscard]] constexpr auto touching(pos_t head, pos_t tail) -> bool {
    auto const dist = abs(head - tail);
    return dist.first <= 1 && dist.second <= 1;
}

[[nodiscard]] constexpr auto manhatten_unit_vector(pos_t v) -> pos_t {
    return {
        v.first == 0 ? 0 : v.first / std::abs(v.first),
        v.second == 0 ? 0 : v.second / std::abs(v.second)
    };
}

[[nodiscard]] constexpr auto tail_calc(pos_t head, pos_t tail) -> pos_t {
    if (touching(head, tail)) {
        return tail;

    } else {
        return tail + manhatten_unit_vector(head - tail);
    }
}

template<int size>
[[nodiscard]] int simulate_rope(auto & cmds) {
    auto visited = std::set<pos_t>{};

    safe::array<pos_t, size> rope{};

    visited.insert(rope.back());

    for (auto const & [cmd_result, dir_val, amt] : cmds) {
        if (cmd_result) {
            for (int step = 0; step < amt; step++) {
                rope[0_s32] = rope[0_s32] + dir_val;

                safe::for_each(1_s32, safe::s32_<size>, [&](auto const knot){
                    rope[knot] = tail_calc(rope[knot - 1_s32], rope[knot]);
                });

                visited.insert(rope.back());
            }
        }
    }

    return visited.size();
}

int main() {
    auto const start = now();

    auto input =
        std::ifstream{"input.txt"};

    auto cmds =
        r::getlines_view(input) |
        rv::transform([](auto line){
            auto const [cmd_result, dir, amt] =
                scn::scan_tuple<char, int>(line, "{} {}");

            auto const dir_val = [](auto dir) -> pos_t {
                switch (dir) {
                    case 'L' : return {-1,  0};
                    case 'R' : return { 1,  0};
                    case 'U' : return { 0, -1};
                    case 'D' : return { 0,  1};
                    default  : return { 0,  0};
                }
            }(dir);

            return std::make_tuple(cmd_result, dir_val, amt);
        }) |
        r::to_vector;

    auto const mid = now();

    fmt::print("part1 answer {}\n", simulate_rope<2>(cmds));
    fmt::print("part2 answer {}\n", simulate_rope<10>(cmds));

    auto const end = now();

    std::cout << "file io time: " << to_nanos(mid - start) << " ns" << std::endl;
    std::cout << "calc time: " << to_nanos(end - mid) << " ns" << std::endl;

    return 0;
}