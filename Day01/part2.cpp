#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <cassert>
#include <range/v3/all.hpp>
#include "utils.hpp"

//200945
int main(int argc, char *argv[])
{
    using namespace ranges;

    //quick and dirty
    assert(argc == 2);
    const std::string input_filename(argv[1]);

    auto input = AoC::get_input(input_filename, "\n\n", [](auto v){
        return accumulate(AoC::split(v, '\n', [](auto x) {
            return std::stoull(x);
        }), 0);
    });

    const auto best_three_sum = accumulate(
                                        std::move(input)
                                        | actions::sort(std::greater<>())
                                        | actions::take(3),
                                        0);
    std::cout << best_three_sum << std::endl;

    return 0;
}
