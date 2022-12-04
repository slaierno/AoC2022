#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>

#include "utils.hpp"

auto get_element_score(auto c) {
    switch (c) {
        case 'a' ... 'z':
            return c - 'a' + 1;
        case 'A' ... 'Z':
            return c - 'A' + 27;
        default:
            throw;
    }
}

// 7766
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    auto priority_sum = accumulate(
        AoC::get_input(argv[1], "\n",
                       [](const std::string_view v) {
                           const std::string_view comp1(
                               v.begin(), v.begin() + (v.size() / 2));
                           const std::string_view comp2(
                               v.begin() + (v.size() / 2), v.end());
                           const auto duplicate = v[comp1.find_first_of(comp2)];
                           return get_element_score(duplicate);
                       }),
        0ULL);

    std::cout << priority_sum << std::endl;

    return 0;
}
