#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
#include <unordered_map>

#include "utils.hpp"
using namespace ranges;

enum class Tile {
    Air,
    Rock,
    Sand,
};

// 728
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    std::unordered_map<AoC::Point<int>, Tile> space;
    const auto input = AoC::get_input(argv[1], "\n", [](auto v) {
        return AoC::split(v, "->", [](auto p) { return AoC::Point(p); });
    });
    int bottom_line = 0;
    for (const auto& lines : input) {
        for (const auto& pair : lines | views::sliding(2)) {
            bottom_line = std::max({bottom_line, pair[0].y, pair[1].y});
            const auto sn = (pair[1] - pair[0]).unit_vec();
            for (auto sp = pair[0]; sp != (pair[1] + sn); sp += sn) {
                space[sp] = Tile::Rock;
            }
        }
    }
    const std::array<AoC::Point<int>, 3> falling_list{
        AoC::Point<int>(0, 1),
        AoC::Point<int>(-1, 1),
        AoC::Point<int>(1, 1),
    };

    const auto sand_start = AoC::Point<int>(500, 0);
    unsigned sand_cnt = 0;

    for (bool reached_bottom = false; !reached_bottom;) {
        for (auto sand = sand_start; !reached_bottom;) {
            const auto it = find_if(falling_list, [&](const auto& f) {
                return space[sand + f] == Tile::Air;
            });
            if (it == falling_list.end()) {
                space[sand] = Tile::Sand;
                sand_cnt++;
                break;
            }
            sand += *it;
            reached_bottom = sand.y > bottom_line;
        }
    }
    std::cout << sand_cnt << std::endl;
    return 0;
}
