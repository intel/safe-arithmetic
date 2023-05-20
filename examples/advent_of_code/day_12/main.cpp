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
constexpr int MAX_STEPS = SIZE_X * SIZE_Y;

using x_int_t = safe::ival_s32<0, SIZE_X - 1>;
using y_int_t = safe::ival_s32<0, SIZE_Y - 1>;

using elev_t = ival_s8<0, 27>;
using steps_t = ival_s32<0, MAX_STEPS>;

constexpr auto to_elev =
    safe::function<elev_t>(
        [](ival_s32<'a', 'z'> c){return c - s32_<'a'>;},
        [](safe::var<char, safe::set<'S'>> c){return 26_s32;},
        [](safe::var<char, safe::set<'E'>> c){return 27_s32;},
        [](){return 0_u32;}
    );

using grid_t = safe::array<safe::array<elev_t, SIZE_X>, SIZE_Y>;
using dist_t = safe::array<safe::array<steps_t, SIZE_X>, SIZE_Y>;


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

point<x_int_t, y_int_t> find(elev_t c, auto const & grid) {
    for (auto const y : safe::irange(0_s32, s32_<SIZE_Y>)) {
        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
            if (grid[y][x] == c) {
                return {x, y};
            }
        }
    }

    return {0_s32, 0_s32};
}


void find_steps(
    safe::Var auto steps,
    safe::Var auto start_x,
    safe::Var auto start_y,
    grid_t const & grid,
    dist_t & dist);


auto safe_find_steps = safe::function<void>(
    [](
        steps_t steps,
        x_int_t start_x,
        y_int_t start_y,
        x_int_t end_x,
        y_int_t end_y,
        grid_t const & grid,
        dist_t & dist
    ){
        dist[start_y][start_x] = steps;

        auto const src_elev = grid[start_y][start_x];
        auto const dst_elev = grid[end_y][end_x];
        auto const elev_diff = dst_elev - src_elev;
        bool const elev_ok = elev_diff > -2_s32;

        auto const src_dist = dist[start_y][start_x];
        auto const dst_dist = dist[end_y][end_x];
        bool const dist_ok = src_dist < (dst_dist - 1_s32);

        if (elev_ok && dist_ok) {
            find_steps(steps + 1_s32, end_x, end_y, grid, dist);
        }
    },
    [](){
        // out of bounds, expected and OK to drop
    }
);


void find_steps(
    safe::Var auto steps,
    safe::Var auto start_x,
    safe::Var auto start_y,
    grid_t const & grid,
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


    grid_t grid{};
    dist_t dist{};

    // clear dist and copy unsafe_grid to safe array grid
    for (auto const y : safe::irange(0_s32, s32_<SIZE_Y>)) {
        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
            grid[y][x] = to_elev(unsafe_grid[y.unsafe_value()][x.unsafe_value()]);
            dist[y][x] = safe::s32_<MAX_STEPS>;
        }
    }

    auto const mid = now();

    auto const start_pt = find(26_s32, grid);
    auto const end_pt = find(27_s32, grid);

    grid[start_pt.y][start_pt.x] = 0_s32;
    grid[end_pt.y][end_pt.x] = 25_s32;

    find_steps(0_s32, end_pt.x, end_pt.y, grid, dist);




    steps_t part2_steps = safe::s32_<MAX_STEPS>;
    for (auto const y : safe::irange(0_s32, s32_<SIZE_Y>)) {
        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
            if (grid[y][x] == 0_s32) {
                part2_steps = min(part2_steps, dist[y][x]);
            }
        }
    }

    auto const end = now();


    for (auto const y : safe::irange(0_s32, s32_<SIZE_Y>)) {
        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
            auto const d = dist[y][x];
            if (d < safe::s32_<MAX_STEPS>) {
                fmt::print("{:4d}", d.unsafe_value());
            } else {
                fmt::print("    ");
            }
        }
        fmt::print("\n");

        for (auto const x : safe::irange(0_s32, s32_<SIZE_X>)) {
            fmt::print("  {:2d}", grid[y][x].unsafe_value());
        }
        fmt::print("\n");
        fmt::print("\n");
    }

    fmt::print("part1 answer {}\n", dist[start_pt.y][start_pt.x].unsafe_value());
    fmt::print("part2 answer {}\n", part2_steps.unsafe_value());

    std::cout << "file io time: " << to_nanos(mid - start) << " ns" << std::endl;
    std::cout << "calc time: " << to_nanos(end - mid) << " ns" << std::endl;

    return 0;
}