#include <safe.hpp>
#include <safe/iostream.hpp>

#include <range/v3/view.hpp>

#include <iostream>
#include <fstream>
#include <string_view>
#include <charconv>

#include <map>


using namespace safe::literals;

using round_score_t = safe::ival_s32<1, 9>;

// A for Rock, B for Paper, and C for Scissors.
// X for Rock, Y for Paper, and Z for Scissors.
std::unordered_map<std::string_view, round_score_t> part1_table {
    {"A X", 1_s32 + 3_s32},
    {"A Y", 2_s32 + 6_s32},
    {"A Z", 3_s32 + 0_s32},
    {"B X", 1_s32 + 0_s32},
    {"B Y", 2_s32 + 3_s32},
    {"B Z", 3_s32 + 6_s32},
    {"C X", 1_s32 + 6_s32},
    {"C Y", 2_s32 + 0_s32},
    {"C Z", 3_s32 + 3_s32}
};

// A for Rock, B for Paper, and C for Scissors.
// X means you need to lose, Y means you need to end the round in a draw, and Z means you need to win.
std::unordered_map<std::string_view, round_score_t> part2_table {
    {"A X", 3_s32 + 0_s32},
    {"A Y", 1_s32 + 3_s32},
    {"A Z", 2_s32 + 6_s32},
    {"B X", 1_s32 + 0_s32},
    {"B Y", 2_s32 + 3_s32},
    {"B Z", 3_s32 + 6_s32},
    {"C X", 2_s32 + 0_s32},
    {"C Y", 3_s32 + 3_s32},
    {"C Z", 1_s32 + 6_s32}
};

auto lookup_score = [](auto const & table){
    return [&](auto x){
        return table.at(x);
    };
};

auto calculate_score(auto const & table) {
    auto input =
        std::ifstream{"input.txt"};

    auto result =
        ranges::getlines_view(input) |
        ranges::views::transform(lookup_score(table));

    auto const score =
        safe::accumulate<3000>(result);

    return score;
}

int main() {
    std::cout << "part 1 score: " << calculate_score(part1_table) << std::endl;
    std::cout << "part 2 score: " << calculate_score(part2_table) << std::endl;

    return 0;
}