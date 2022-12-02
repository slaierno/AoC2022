#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>

#include "utils.hpp"

constexpr unsigned str_to_num(const std::string_view str) {
    return ((str[0] - 'A') << 2) | (str[2] - 'X');
}

constexpr auto result_map = []() {
    std::array<unsigned, 16> result_score{};
    const std::array<std::string_view, 9> possible_rounds = {
        "A X", "A Y", "A Z",  //
        "B X", "B Y", "B Z",  //
        "C X", "C Y", "C Z",
    };
    for (const auto& round : possible_rounds) {
        const auto opponent = round[0] - 'A';
        const auto winlose = round[2] - 'X';
        const auto you = (opponent + (winlose - 1 + 3)) % 3;
        result_score[str_to_num(round)] = 3 * winlose + you + 1;
    }
    return result_score;
}();

// 14204
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    const auto score = accumulate(
        AoC::get_input(argv[1], "\n",
                       [](auto v) { return result_map[str_to_num(v)]; }),
        0ULL);

    std::cout << score << std::endl;

    return 0;
}