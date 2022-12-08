#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
#include <unordered_set>

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
    std::unordered_set<size_t> visibility_set{};
    // std::unordered_set<size_t> unvisibility_set{};
    // auto is_visible = [&visibility_set, w](auto i, auto j) {
    //     return visibility_set.end() != visibility_set.find(i * w + j);
    // };
    auto set_visible = [&visibility_set, w](auto i, auto j) {
        return visibility_set.insert(i * w + j);
    };
    auto is_edge = [w, h](auto i, auto j) {
        return (0 == i || 0 == j || (h - 1) == i || (w - 1) == j);
    };
    // auto is_unvisible = [&unvisibility_set, w](auto i, auto j) {
    //     return unvisibility_set.end() != unvisibility_set.find(i * w + j);
    // };
    // auto set_unvisible = [&unvisibility_set, w](auto i, auto j) {
    //     return unvisibility_set.insert(i * w + j);
    // };
    for (int i = 0; i < h; i++) {
        set_visible(i, 0);
        set_visible(i, w - 1);
    }
    for (int j = 0; j < w; j++) {
        set_visible(0, j);
        set_visible(h - 1, j);
    }
    for (int i = 1; i < h - 1; i++) {
        for (int j = 1; j < w - 1; j++) {
            bool visible = false;
            // From top
            for (int iv = i - 1; iv >= 0; iv--) {
                if (input[iv][j] >= input[i][j])
                    break;
                else if (is_edge(iv, j)) {
                    visible = true;
                    break;
                }
            }
            if (visible) {
                set_visible(i, j);
                continue;
            }
            // From bottom
            for (int iv = i + 1; iv < h; iv++) {
                if (input[iv][j] >= input[i][j])
                    break;
                else if (is_edge(iv, j)) {
                    visible = true;
                    break;
                }
            }
            if (visible) {
                set_visible(i, j);
                continue;
            }
            // From left
            for (int jv = j - 1; jv >= 0; jv--) {
                if (input[i][jv] >= input[i][j])
                    break;
                else if (is_edge(i, jv)) {
                    visible = true;
                    break;
                }
            }
            if (visible) {
                set_visible(i, j);
                continue;
            }
            // From right
            for (int jv = j + 1; jv < w; jv++) {
                if (input[i][jv] >= input[i][j])
                    break;
                else if (is_edge(i, jv)) {
                    visible = true;
                    break;
                }
            }
            if (visible) {
                set_visible(i, j);
                continue;
            }
        }
    }
    std::cout << visibility_set.size() << std::endl;
    return 0;
}
