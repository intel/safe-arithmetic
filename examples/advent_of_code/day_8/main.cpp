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



// https://adventofcode.com/2022/day/8

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

enum class fs_node_type {
    DIRECTORY,
    FILE
};

int is_tree_visible(auto & map, int x, int y) {
    auto const my_height = map[x][y];

    if (
        x == 0 || x == 98 ||
        y == 0 || y == 98
    ) {
        return 1;
    }

    bool const visible_from_left = [&](){
        for (int i = 0; i < x; i++) {
            if (map[i][y] >= my_height) {
                return false;
            }
        }

        return true;
    }();

    if (visible_from_left) {
        return 1;
    }

    bool const visible_from_right = [&](){
        for (int i = 98; i > x; i--) {
            if (map[i][y] >= my_height) {
                return false;
            }
        }

        return true;
    }();

    if (visible_from_right) {
        return 1;
    }

    bool const visible_from_top = [&](){
        for (int i = 0; i < y; i++) {
            if (map[x][i] >= my_height) {
                return false;
            }
        }

        return true;
    }();

    if (visible_from_top) {
        return 1;
    }

    bool const visible_from_bottom = [&](){
        for (int i = 98; i > y; i--) {
            if (map[x][i] >= my_height) {
                return false;
            }
        }

        return true;
    }();

    if (visible_from_bottom) {
        return 1;
    }

    return 0;
}


int calc_scenic_score(auto & map, int x, int y) {
    auto const my_height = map[x][y];

    int const visible_from_left = [&](){
        int count = 0;
        for (int i = x + 1; i < 99; i++) {
            count++;

            if (map[i][y] >= my_height) {
                return count;
            }
        }

        return count;
    }();

    int const visible_from_right = [&](){
        int count = 0;
        for (int i = x - 1; i >= 0; i--) {
            count++;

            if (map[i][y] >= my_height) {
                return count;
            }
        }

        return count;
    }();

    int const visible_from_top = [&](){
        int count = 0;
        for (int i = y + 1; i < 99; i++) {
            count++;

            if (map[x][i] >= my_height) {
                return count;
            }
        }

        return count;
    }();

    int const visible_from_bottom = [&](){
        int count = 0;
        for (int i = y - 1; i >= 0; i--) {
            count++;

            if (map[x][i] >= my_height) {
                return count;
            }
        }

        return count;
    }();

    return
        visible_from_top *
        visible_from_bottom *
        visible_from_left *
        visible_from_right;
}

int main() {
    auto const start = now();

    auto input =
        std::ifstream{"input.txt"};

    auto map =
        r::getlines_view(input) |
        r::to_vector;

    auto const mid = now();

    int visible_trees = 0;
    for (int x = 0; x < 99; x++) {
        for (int y = 0; y < 99; y++) {
            visible_trees += is_tree_visible(map, x, y);
        }
    }

    fmt::print("part1 answer {}\n", visible_trees);


    int best_scenic_score = 0;
    for (int x = 0; x < 99; x++) {
        for (int y = 0; y < 99; y++) {
            best_scenic_score = std::max<int>(best_scenic_score, calc_scenic_score(map, x, y));
        }
    }

    fmt::print("part2 answer {}\n", best_scenic_score);


    auto const end = now();

    std::cout << "file io time: " << to_nanos(mid - start) << " ns" << std::endl;
    std::cout << "calc time: " << to_nanos(end - mid) << " ns" << std::endl;

    return 0;
}