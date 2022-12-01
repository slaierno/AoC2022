#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <range/v3/all.hpp>
#include "utils.hpp"

//69693
int main(int argc, char *argv[])
{
    using namespace ranges;

    //quick and dirty
    assert(argc == 2);
    const std::string input_filename(argv[1]);

    auto input = AoC::get_input(input_filename, "\n\n", [](auto v){
        return ranges::accumulate(AoC::split(v, '\n', [](auto x) {
            return std::stoull(x);
        }), 0);
    });

    const auto best_sum = (std::move(input) | actions::sort).back();
    std::cout << best_sum << std::endl;

    return 0;
}
