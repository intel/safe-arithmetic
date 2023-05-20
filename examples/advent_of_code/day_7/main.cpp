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



// https://adventofcode.com/2022/day/7

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

struct fs_node {
private:
    fs_node_type type_;
    std::string name_;
    std::size_t size_;
    std::map<std::string, fs_node *> children_;

public:
    fs_node(
        fs_node_type type,
        std::string name,
        std::size_t size
    )
        : type_{type}
        , name_{name}
        , size_{size}
        , children_{}
    {}

    [[nodiscard]] fs_node * get(std::string const & name) {
        return children_.at(name);
    }

    void add(fs_node * child) {
        children_[child->name()] = child;
    }

    [[nodiscard]] std::string const & name() const {
        return name_;
    }

    [[nodiscard]] std::size_t size() const {
        return size_;
    }

    [[nodiscard]] fs_node_type type() const {
        return type_;
    }

    auto begin() {
        return children_.begin();
    }

    auto end() {
        return children_.end();
    }
};

std::size_t rsize(fs_node * n) {
    auto const children_size = std::accumulate(n->begin(), n->end(), std::size_t{}, [](std::size_t size, auto b){
        return size + rsize(b.second);
    });

    return children_size + n->size();
}


void walk(fs_node * n, auto fn) {
    fn(n);

    for (auto p: *n) {
        auto * child = p.second;
        walk(child, fn);
    }
}

int main() {
    auto const start = now();

    auto input =
        std::ifstream{"input.txt"};

    auto const mid = now();

    fs_node root{
        fs_node_type::DIRECTORY,
        "/",
        0u
    };

    std::vector<fs_node *> path{};

    path.push_back(&root);

    for (auto const line : r::getlines_view(input)) {
        fs_node * cwd = path.back();

        auto const [cd_result, cd_name] =
            scn::scan_tuple<std::string>(line, "$ cd {}");

        if (cd_result) {
            if (cd_name == "/") {
                path.clear();
                path.push_back(&root);

            } else if (cd_name == "..") {
                path.pop_back();

            } else {
                auto * dst = cwd->get(cd_name);
                path.push_back(dst);
            }
        }

        auto const [file_result, file_size, file_name] =
            scn::scan_tuple<std::size_t, std::string>(line, "{} {}");

        if (file_result) {
            auto * child = new fs_node{
                fs_node_type::FILE,
                file_name,
                file_size
            };

            cwd->add(child);
        }

        auto const [dir_result, dir_name] =
            scn::scan_tuple<std::string>(line, "dir {}");

        if (dir_result) {
            auto * child = new fs_node{
                fs_node_type::DIRECTORY,
                dir_name,
                0u
            };

            cwd->add(child);
        }
    }

    auto part1_answer = std::size_t{};
    walk(&root, [&](fs_node * n){
        auto const size = rsize(n);

        if (
            n->type() == fs_node_type::DIRECTORY &&
            size <= 100000
        ) {
            part1_answer += size;
        }
    });

    fmt::print("part1 answer {}\n", part1_answer);

    auto const used_space = rsize(&root);
    auto const free_space = 70000000 - used_space;

    auto part2_answer = std::numeric_limits<std::size_t>::max();
    walk(&root, [&](fs_node * n){
        auto const size = rsize(n);

        if (
            n->type() == fs_node_type::DIRECTORY &&
            size >= (30000000 - free_space)
        ) {
            part2_answer = std::min(part2_answer, size);
        }
    });

    fmt::print("part2 answer {}\n", part2_answer);

    auto const end = now();

    std::cout << "file io time: " << to_nanos(mid - start) << " ns" << std::endl;
    std::cout << "calc time: " << to_nanos(end - mid) << " ns" << std::endl;

    return 0;
}