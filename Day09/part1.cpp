#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
#include <set>
#include <string>

#include "utils.hpp"

struct Instruction {
    AoC::Point direction;
    unsigned steps;

    Instruction(const std::string& str)
        : direction(dir_to_point(str[0])), steps(std::stoull(str.substr(2))) {}

    static AoC::Point dir_to_point(char c) {
        return {(c == 'R')   ? 1
                : (c == 'L') ? -1
                             : 0,
                (c == 'U')   ? 1
                : (c == 'D') ? -1
                             : 0};
    }
};

// 6081
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    const auto input = AoC::get_input(
        argv[1], '\n', [](const auto& v) { return Instruction(v); });

    AoC::Point head{0, 0};
    AoC::Point tail{0, 0};
    std::set<AoC::Point> tail_cover_map{tail};

    for (const auto& [dir, n_of_steps] : input) {
        for (int steps = n_of_steps; steps--;) {
            head += dir;
            if (auto diff = head - tail; diff.sup_norm() > 1) {
                tail += diff.unit_vec();
                tail_cover_map.insert(tail);
            }
        }
    }

    std::cout << tail_cover_map.size() << std::endl;
    return 0;
}
