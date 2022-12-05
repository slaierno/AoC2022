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

    const auto input = AoC::get_input(argv[1], "\n\n");
    const auto stack_str = AoC::split(input[0], "\n");
    const auto instr_list =
        AoC::split(input[1], "\n", [](const std::string& str) {
            const auto mft = std::move(str) | actions::split(' ');
            return std::tuple{std::stoull(mft[1]), std::stoull(mft[3]) - 1,
                              std::stoull(mft[5]) - 1};
        });

    const size_t num_of_stacks = (stack_str[0].size() + 1) / 4;
    std::vector<std::stack<char>> hanoi(num_of_stacks);
    for (const auto& level : stack_str | views::drop_last(1) | views::reverse) {
        for (unsigned i = 0; i < num_of_stacks; i++) {
            if (const auto crate = level[4 * (i + 1) - 3]; crate != ' ') {
                hanoi[i].push(crate);
            }
        }
    }

    for (auto [move, from, to] : instr_list) {
        while (move--) {
            hanoi[to].push(hanoi[from].top());
            hanoi[from].pop();
        }
    }
    for (const auto& s : hanoi) std::cout << s.top();
    std::cout << std::endl;
    return 0;
}
