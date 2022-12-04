#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>

#include "utils.hpp"

// 891
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
    const auto overlapping_cnt = count_if(input, [](const auto& couple) {
        return couple[0][0] <= couple[1][1] && couple[1][0] <= couple[0][1];
    });

    std::cout << overlapping_cnt << std::endl;

    return 0;
}
