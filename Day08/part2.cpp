#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
#include <set>

#include "utils.hpp"

// 1300
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    const auto input = AoC::get_input(argv[1], '\n', [](const auto& v) {
        return v |
               views::transform([](auto c) -> unsigned { return c - '0'; }) |
               to<std::vector>();
    });

    const int h = input.size();
    const int w = input[0].size();
    std::set<size_t> score_set{};
    for (int i = 1; i < h - 1; i++) {
        for (int j = 1; j < w - 1; j++) {
            unsigned score = 1;
            unsigned visible = 0;
            // From top
            for (int iv = i - 1; iv >= 0; iv--) {
                visible++;
                if (input[iv][j] >= input[i][j]) break;
            }
            score *= visible;
            visible = 0;
            // From bottom
            for (int iv = i + 1; iv < h; iv++) {
                visible++;
                if (input[iv][j] >= input[i][j]) break;
            }
            score *= visible;
            visible = 0;
            // From left
            for (int jv = j - 1; jv >= 0; jv--) {
                visible++;
                if (input[i][jv] >= input[i][j]) break;
            }
            score *= visible;
            visible = 0;
            // From right
            for (int jv = j + 1; jv < w; jv++) {
                visible++;
                if (input[i][jv] >= input[i][j]) break;
            }
            score *= visible;
            score_set.insert(score);
        }
    }
    std::cout << *score_set.rbegin() << std::endl;
    return 0;
}
