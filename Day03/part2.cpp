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

// 2415
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    const auto input = AoC::get_input(argv[1], "\n");
    const auto priority_sum =
        accumulate(input | views::chunk(3) | views::transform([](auto v3) {
                       auto el = find_if(v3[0], [&v3](auto c) {
                           return v3[1].contains(c) && v3[2].contains(c);
                       });
                       if (el == v3[0].end()) throw;
                       return get_element_score(*el);
                   }),
                   0ULL);

    std::cout << priority_sum << std::endl;

    return 0;
}
