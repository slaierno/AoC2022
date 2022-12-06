#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>

#include "utils.hpp"

// 1300
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    constexpr size_t marker_size = 4;
    const auto input = AoC::get_input(argv[1]);
    for (auto [i, window] :
         input | views::sliding(marker_size) | views::enumerate) {
        if (size(window) == size(window | to<std::vector>() | actions::sort |
                                 actions::unique)) {
            std::cout << i + marker_size << std::endl;
            return 0;
        }
    }
    std::cout << "ERROR: no marker found!" << std::endl;
    return -1;
}
