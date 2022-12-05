#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
#include <stack>
#include <string_view>

#include "utils.hpp"

// BWNCQRMDB
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    std::array<std::stack<char>, 9> hanoi{};
    const auto input = AoC::get_input(argv[1], "\n\n");
    const auto stack_str = AoC::split(input[0], "\n");
    const auto instr_list =
        AoC::split(input[1], "\n", [](const std::string& str) {
            const auto mft = std::move(str) | actions::split(' ');
            return std::tuple{std::stoull(mft[1]), std::stoull(mft[3]) - 1,
                              std::stoull(mft[5]) - 1};
        });

    for (const auto& level : stack_str | views::drop_last(1) | views::reverse) {
        for (unsigned i = 0; i < 10; i++) {
            if (const auto crate = level[4 * (i + 1) - 3]; crate != ' ') {
                hanoi[i].push(crate);
            }
        }
    }

    for (const auto& [move, from, to] : instr_list) {
        for (unsigned i = 0; i < move; i++) {
            hanoi[to].push(hanoi[from].top());
            hanoi[from].pop();
        }
    }
    for (const auto& s : hanoi) {
        std::cout << s.top();
    }
    std::cout << std::endl;
    return 0;
}
