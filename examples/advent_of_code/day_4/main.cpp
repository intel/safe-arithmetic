#include <safe.hpp>
#include <safe/iostream.hpp>

#include <range/v3/view.hpp>
#include <range/v3/action.hpp>
#include <range/v3/algorithm.hpp>

#include <iostream>
#include <fstream>
#include <string_view>
#include <chrono>

// https://adventofcode.com/2022/day/3

using namespace safe::literals;
using namespace safe::int_types;
using namespace safe::interval_types;

namespace r = ranges;
namespace rv = ranges::views;

constexpr auto & now = std::chrono::steady_clock::now;

using letter_score_t = ival_u32<0, 52>;
using bag_mask_t = safe::var<uint64_t, safe::mask<0x1f'ffff'ffff'ffffu>>;

constexpr auto to_priority =
    safe::match<letter_score_t>(
        [](ival_s32<'a', 'z'> c){return c - s32_<'a'> +  1_u32;},
        [](ival_s32<'A', 'Z'> c){return c - s32_<'A'> + 27_u32;},
        [](){return 0_u32;}
    );

constexpr auto to_one_hot = [](letter_score_t score) -> bag_mask_t {
    return 1_u64 << score;
};

constexpr auto to_bit_mask = [](auto bag_str) -> bag_mask_t {
    auto const masks =
        bag_str |
        rv::transform(to_priority) |
        rv::transform(to_one_hot);

    return r::fold_left(
        masks,
        bag_mask_t{0_u64},
        std::bit_or<bag_mask_t>());
};

auto find_common_item = [](auto bag_masks){
    auto const common_item= r::fold_left(
        bag_masks,
        bag_mask_t{u64_<0x1f'ffff'ffff'ffff>},
        std::bit_and<bag_mask_t>());

    return bit_width(common_item >> 1_u32);
};

auto split_bags = [](std::string_view bag) {
    return
        bag |
        rv::chunk(bag.size() / 2) |
        rv::transform(to_bit_mask);
};

constexpr auto to_micros = [](auto val) {
    return std::chrono::duration_cast<std::chrono::microseconds>(val).count();
};

int main() {
    auto const start = now();

    auto input =
        std::ifstream{"input.txt"};

    auto lines =
        r::getlines_view(input) |
        r::to_vector;

    auto const mid = now();

    auto const score1 =
        safe::accumulate<500>(
            lines |
            rv::transform(split_bags) |
            rv::transform(find_common_item)
        );

    auto const score2 =
        safe::accumulate<500>(
            lines |
            rv::transform(to_bit_mask) |
            rv::chunk(3) |
            rv::transform(find_common_item)
        );

    auto const end = now();

    std::cout << "score part 1: " << score1 << std::endl;
    std::cout << "score part 2: " << score2 << std::endl;

    std::cout << "file io time: " << to_micros(mid - start) << " us" << std::endl;
    std::cout << "calc time: " << to_micros(end - mid) << " us" << std::endl;

    return 0;
}