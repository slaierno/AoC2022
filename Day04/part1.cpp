#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>

#include "utils.hpp"

// 602
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    const auto input =
        AoC::get_input(argv[1], "\n", [](const auto& couple_str) {
            return AoC::split(couple_str, ',', [](const auto rng) {
                return AoC::split(rng, '-', [](auto num_str) {
                    return std::stoull(num_str);
                });
            });
        });
    const auto overlapping_cnt = cpp20::count_if(input, [](const auto& couple) {
        const auto contains = [](const auto& first, const auto& second) {
            return (first[0] <= second[0] && first[1] >= second[1]);
        };
        return contains(couple[0], couple[1]) || contains(couple[1], couple[0]);
    });

    std::cout << overlapping_cnt << std::endl;

    return 0;
}
