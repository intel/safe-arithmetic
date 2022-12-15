#include <safe.hpp>
#include <safe/iostream.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>

#include <range/v3/view.hpp>
#include <range/v3/action.hpp>

// https://adventofcode.com/2022/day/1

using namespace safe::literals;

using item_int_t = safe::ival_s32<0, 99999>;

constexpr auto to_safe = [](auto calorie_pack) {
    std::vector<item_int_t> data{};

    for (auto const line : calorie_pack) {
        safe::function<void>([&](item_int_t calories){
            data.push_back(calories);
        })(std::stoi(line));
    }

    return data;
};

constexpr auto empty_line = [](auto line){
    return line.empty();
};

constexpr auto accumulate_calories = [](auto x){
    return safe::accumulate<100>(x);
};

int main() {
    auto input_file =
        std::ifstream{"input.txt"};

    auto lines =
        ranges::getlines_view(input_file) |
        ranges::to<std::vector>();

    auto elf_calories =
        lines |
        ranges::views::split_when(empty_line) |
        ranges::views::transform(to_safe) |
        ranges::views::transform(accumulate_calories) |
        ranges::to<std::vector>();

    ranges::sort(elf_calories);

    auto top_one = (
        elf_calories |
        ranges::views::take_last(1)
    )[0];

    auto top_three = safe::accumulate<3>(
        elf_calories |
        ranges::views::take_last(3)
    );

    std::cout << "top_one: " << top_one << std::endl;
    std::cout << "top_three: " << top_three << std::endl;

    return 0;
}