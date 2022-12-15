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

// https://adventofcode.com/2022/day/4

using namespace safe::literals;
using namespace safe::int_types;
using namespace safe::interval_types;

namespace r = ranges;
namespace rv = ranges::views;

constexpr auto & now = std::chrono::steady_clock::now;

constexpr auto to_micros = [](auto val) {
    return std::chrono::duration_cast<std::chrono::microseconds>(val).count();
};

using overlap_t = ival_s32<0, 1>;
using section_id_t = safe::s32;

struct section_range {
    section_id_t min;
    section_id_t max;
};

using elf_pair_t = std::pair<section_range, section_range>;

constexpr auto parse_pair = [](std::string_view str) -> elf_pair_t {
    auto const [result, lhs_min, lhs_max, rhs_min, rhs_max] =
        scn::scan_tuple<int, int, int, int>(str, "{}-{},{}-{}");

    return {
        {safe::value(lhs_min), safe::value(lhs_max)},
        {safe::value(rhs_min), safe::value(rhs_max)}
    };
};

constexpr auto contains = [](section_range lhs, section_range rhs) -> overlap_t {
    return safe::value((lhs.min <= rhs.min) && (lhs.max >= rhs.max));
};

constexpr auto score_overlap1 = [](elf_pair_t elves) -> overlap_t {
    return
        contains(elves.first, elves.second) |
        contains(elves.second, elves.first);
};

constexpr auto score_overlap2 = [](elf_pair_t elves) -> overlap_t {
    return safe::value(
        (elves.second.min <= elves.first.max) &&
        (elves.first.min <= elves.second.max)
    );
};

int main() {
    auto const start = now();

    auto input =
        std::ifstream{"input.txt"};

    auto pairs =
        r::getlines_view(input) |
        rv::transform(parse_pair) |
        r::to_vector;

    auto const mid = now();

    auto const score1 = safe::accumulate<1000>(pairs | rv::transform(score_overlap1));
    auto const score2 = safe::accumulate<1000>(pairs | rv::transform(score_overlap2));

    auto const end = now();

    std::cout << "score part 1: " << score1 << std::endl;
    std::cout << "score part 2: " << score2 << std::endl;

    std::cout << "file io time: " << to_micros(mid - start) << " us" << std::endl;
    std::cout << "calc time: " << to_micros(end - mid) << " us" << std::endl;

    return 0;
}