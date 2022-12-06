#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
#include <set>

#include "utils.hpp"

using namespace ranges;

bool is_unique_c26(const ranges::input_range auto& window) {
    std::array<char, 26> cs{};
    return window.end() ==
           find_if(window, [&cs](auto c) { return ++cs[c - 'a'] > 1; });
}
bool is_unique(const ranges::input_range auto& window) {
    std::set<char> cs{};
    return window.end() ==
           find_if(window, [&cs](auto c) { return !cs.insert(c).second; });
}

// 3986
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    constexpr size_t marker_size = 4;
    const auto input = AoC::get_input(argv[1]);
    for (const auto& [i, window] :
         input | views::sliding(marker_size) | views::enumerate) {
        if (is_unique_c26(window)) {
            std::cout << i + marker_size << std::endl;
            return 0;
        }
    }
    std::cout << "ERROR: no marker found!" << std::endl;
    return -1;
}
