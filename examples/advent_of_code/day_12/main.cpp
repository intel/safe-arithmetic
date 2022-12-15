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



// https://adventofcode.com/2022/day/12

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

constexpr int SIZE_X = 154;
constexpr int SIZE_Y = 41;

using x_int_t = safe::ival_s32<0, SIZE_X - 1>;
using y_int_t = safe::ival_s32<0, SIZE_Y - 1>;

template<typename XT, typename YT>
struct point {
    XT x;
    YT y;

    template<typename XU, typename YU>
    auto operator+(point<XU, YU> rhs) const {
        return point<
            decltype(x + rhs.x),
            decltype(y + rhs.y)
        >{
            x + rhs.x,
            y + rhs.y
        };
    }

    bool operator==(point rhs) const {
        return x == rhs.x && y == rhs.y;
    }
};

point<x_int_t, y_int_t> find(char c, auto const & grid) {
    for (auto const y : safe::irange(0_s32, s32_<SIZE_Y>)) {
        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
            if (grid[y][x] == c) {
                return {x, y};
            }
        }
    }

    return {0_s32, 0_s32};
}

using grid_t = safe::array<safe::array<std::string::value_type , SIZE_X>, SIZE_Y>;
using dist_t = safe::array<safe::array<int, SIZE_X>, SIZE_Y>;


void find_steps(
    int steps,
    safe::Var auto start_x,
    safe::Var auto start_y,
    grid_t & grid,
    dist_t & dist);

void safe_find_steps(
    int steps,
    safe::Var auto unbounded_start_x,
    safe::Var auto unbounded_start_y,
    safe::Var auto unbounded_end_x,
    safe::Var auto unbounded_end_y,
    grid_t & grid,
    dist_t & dist
) {
    safe::function<void>(
        [&](
            x_int_t start_x,
            y_int_t start_y,
            x_int_t end_x,
            y_int_t end_y
        ){
            dist[start_y][start_x] = steps;

            auto const src_elev = grid[start_y][start_x];
            auto const dst_elev = grid[end_y][end_x];
            auto const elev_diff = dst_elev - src_elev;
            bool const elev_ok = elev_diff > -2;

            auto const src_dist = dist[start_y][start_x];
            auto const dst_dist = dist[end_y][end_x];
            bool const dist_ok = src_dist < (dst_dist - 1);

            if (elev_ok && dist_ok) {
                find_steps(steps + 1, end_x, end_y, grid, dist);
            }
        }
    )(
        unbounded_start_x,
        unbounded_start_y,
        unbounded_end_x,
        unbounded_end_y
    );
}

void find_steps(
    int steps,
    safe::Var auto start_x,
    safe::Var auto start_y,
    grid_t & grid,
    dist_t & dist
) {
    safe_find_steps(steps, start_x, start_y, start_x + 1_s32, start_y, grid, dist);
    safe_find_steps(steps, start_x, start_y, start_x - 1_s32, start_y, grid, dist);
    safe_find_steps(steps, start_x, start_y, start_x, start_y + 1_s32, grid, dist);
    safe_find_steps(steps, start_x, start_y, start_x, start_y - 1_s32, grid, dist);
}


int main() {
    auto const start = now();

    auto input =
        std::ifstream{"input.txt"};

    auto unsafe_grid =
        r::getlines_view(input) |
        r::to<std::vector>();

    auto const mid = now();

    grid_t grid{};
    dist_t dist{};

    // clear dist and copy to safe array grid
    for (auto const y : safe::irange(0_s32, s32_<SIZE_Y>)) {
        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
            grid[y][x] = unsafe_grid[y.unsafe_value()][x.unsafe_value()];
            dist[y][x] = 999'999'999;
        }
    }

    auto const start_pt = find('S', grid);
    auto const end_pt = find('E', grid);

    grid[start_pt.y][start_pt.x] = 'a';
    grid[end_pt.y][end_pt.x] = 'z';

    find_steps(0, end_pt.x, end_pt.y, grid, dist);

//    for (auto const y : safe::irange(0_s32, s32_<SIZE_Y>)) {
//        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
//            auto const d = dist[y][x];
//            if (d < 999'999'999) {
//                fmt::print("{:4d}", d);
//            } else {
//                fmt::print("    ");
//            }
//        }
//        fmt::print("\n");
//
//        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
//            fmt::print("   {:c}", grid[y][x]);
//        }
//        fmt::print("\n");
//        fmt::print("\n");
//    }

    fmt::print("part1 answer {}\n", dist[start_pt.y][start_pt.x]);


    int part2_steps = 999'999'999;
    for (auto const y : safe::irange(0_s32, s32_<SIZE_Y>)) {
        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
            if (grid[y][x] == 'a') {
                part2_steps = std::min(part2_steps, dist[y][x]);
            }
        }
    }
    fmt::print("part2 answer {}\n", part2_steps);

    auto const end = now();

    std::cout << "file io time: " << to_nanos(mid - start) << " ns" << std::endl;
    std::cout << "calc time: " << to_nanos(end - mid) << " ns" << std::endl;

    return 0;
}