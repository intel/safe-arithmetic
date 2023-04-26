#include <safe.hpp>
#include <safe/iostream.hpp>

#include <range/v3/view.hpp>
#include <range/v3/action.hpp>

#include <scn/scn.h>
#include <scn/tuple_return.h>

#include <iostream>
#include <fstream>
#include <string_view>
#include <chrono>
#include <charconv>
#include <queue>

// https://adventofcode.com/2022/day/5

using namespace safe::literals;
using namespace safe::int_types;
using namespace safe::interval_types;

namespace r = ranges;
namespace rv = ranges::views;
namespace ra = ranges::actions;

constexpr auto & now = std::chrono::steady_clock::now;

constexpr auto to_micros = [](auto val) {
    return std::chrono::duration_cast<std::chrono::microseconds>(val).count();
};

constexpr auto to_nanos = [](auto val) {
    return std::chrono::duration_cast<std::chrono::nanoseconds>(val).count();
};


int main() {
    auto const start = now();

    auto input =
        std::ifstream{"input.txt"};

    auto lines =
        r::getlines_view(input) |
        r::to_vector;

    auto const mid = now();

    auto stack_init =
        lines |
        rv::take(10) |
        r::to_vector;

    auto stacks = safe::array<std::vector<char>, 9>{};

    for (auto row : safe::irange(0_s32, 8_s32)) {
        for (auto stack_index : safe::irange(0_s32, 9_s32)) {
            auto const col = 1_s32 + (stack_index * 4_s32);
            auto const crate = stack_init[row.unsafe_value()][col.unsafe_value()];
            auto & stack = stacks[stack_index];

            if (crate != ' ') {
                stack.insert(stack.begin(), crate);
            }
        }
    }

    std::vector<char> temp{};

    for (auto const cmd : lines | rv::drop(10)) {
        auto const [result, unsafe_count, unsafe_src_index, unsafe_dst_index] =
            scn::scan_tuple<int, int, int>(cmd, "move {} from {} to {}");

        if (result) {
            safe::function<void>(
                [&stacks](
                    ival_u32<0, 100> count,
                    ival_u32<1, 9> src_index,
                    ival_u32<1, 9> dst_index
                ){
                    auto & src = stacks[src_index - 1_u32];
                    auto & dst = stacks[dst_index - 1_u32];

//                    for (int i = 0; i < count; i++) {
//                        dst.push_back(src.back());
//                        src.pop_back();
//                    }

                    dst.insert(dst.end(), src.end() - count.unsafe_value(), src.end());
                    src.resize(src.size() - count.unsafe_value());
                },
                [](){}
            )(unsafe_count, unsafe_src_index, unsafe_dst_index);
        }
    }

    auto const end = now();

    for (auto stack : stacks) {
        std::cout << stack.back();
    }
    std::cout << std::endl << std::endl;

    std::cout << "file io time: " << to_nanos(mid - start) << " ns" << std::endl;
    std::cout << "calc time: " << to_nanos(end - mid) << " ns" << std::endl;

    return 0;
}