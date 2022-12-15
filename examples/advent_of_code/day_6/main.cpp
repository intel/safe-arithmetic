#include <safe.hpp>
#include <safe/iostream.hpp>

#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
#include <range/v3/algorithm.hpp>

#include <scn/scn.h>
#include <scn/tuple_return.h>

#include <iostream>
#include <fstream>
#include <string_view>
#include <chrono>
#include <charconv>
#include <bit>


// https://adventofcode.com/2022/day/6

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

using sop_mask_t = safe::var<uint32_t, safe::mask<0x3ff'ffffu>>;

constexpr auto to_one_hot =
    safe::function<sop_mask_t>(
        [](ival_s32<'a', 'z'> c){return 1_u32 << (c - s32_<'a'>);},
        [](){return 0_u32;}
    );

constexpr auto count_unique =
    [](auto seq) {
        constexpr auto bit_or = [](auto a, auto b){return a | b;};
        sop_mask_t const mask = safe::accumulate<100>(seq, bit_or);
        return std::popcount(mask.unsafe_value());
    };

constexpr auto find_first_unique_chunk =
    [](auto seq, safe::u32 size) {
        auto const meq =
            seq |
            rv::transform(to_one_hot) |
            rv::sliding(size.unsafe_value()) |
            rv::transform(count_unique);

        auto const som_iter = r::find(meq.begin(), meq.end(), size.unsafe_value());
        auto const som_index = r::distance(meq.begin(), som_iter) + size.unsafe_value();
        return som_index;
    };

int main() {
    auto const start = now();

    std::string stream{};
    std::getline(std::ifstream{"input.txt"}, stream);

    auto const mid = now();

    auto const sop_index = find_first_unique_chunk(stream, 4_u32);
    auto const som_index = find_first_unique_chunk(stream, 14_u32);

    auto const end = now();

    std::cout << "score part 1: " << sop_index << std::endl;
    std::cout << "score part 2: " << som_index << std::endl;

    std::cout << "file io time: " << to_nanos(mid - start) << " ns" << std::endl;
    std::cout << "calc time: " << to_nanos(end - mid) << " ns" << std::endl;

    return 0;
}