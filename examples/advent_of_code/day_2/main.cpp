#include <safe.hpp>

#include <iostream>
#include <fstream>
#include <string>
#include <charconv>
#include <vector>
#include <algorithm>
#include <ranges>
#include <type_traits>


using namespace safe::literals;

using item_int_t = safe::ival_s32<0, 99999>;

auto parse_elf_data(std::string filename) {
    std::ifstream ifs{filename};
    std::vector<std::vector<item_int_t>> data{};

    while (!ifs.eof()) {
        std::vector<item_int_t> elf_calorie_list{};
        while (!ifs.eof()) {
            std::string line{};
            std::getline(ifs, line);

            if (line.empty()) {
                break;
            }

            safe::invoke([&](item_int_t calories){
                elf_calorie_list.push_back(calories);
            }, std::stoi(line));
        }
        data.push_back(elf_calorie_list);
    }

    return data;
}

int main() {
    auto const elves = parse_elf_data("input.txt");
    std::vector<safe::s32> elf_calories{};

    for (auto const elf : elves) {
        auto const calories = safe::accumulate<100>(elf.begin(), elf.end());
        elf_calories.push_back(calories);
    }

    std::sort(elf_calories.rbegin(), elf_calories.rend());

    std::cout << "max_single_elf: " << elf_calories.front().unsafe_value() << std::endl;
    std::cout << "max_three_elf: " << elf_calories[0].unsafe_value() + elf_calories[1].unsafe_value() + elf_calories[2].unsafe_value() << std::endl;

    return 0;
}