#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>

#include "utils.hpp"

using namespace ranges;

bool is_unique(const ranges::input_range auto& window) {
    std::array<char, 8 * sizeof(char)> cs{};
    return none_of(window, [&cs](auto c) { return ++cs[c] > 1; });
}

// 3986
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    constexpr size_t marker_size = 4;
    const auto input = AoC::get_input(argv[1]);
    for (const auto& [i, window] :
         input | views::sliding(marker_size) | views::enumerate) {
        if (is_unique(window)) {
            std::cout << i + marker_size << std::endl;
            return 0;
        }
    }
    std::cout << "ERROR: no marker found!" << std::endl;
    return -1;
}
