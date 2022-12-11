#include <cassert>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <optional>
#include <queue>
#include <range/v3/all.hpp>

#include "utils.hpp"
using namespace ranges;

enum struct Operator {
    plus,
    divided,
    times,
    minus,
};

using Operand = std::optional<uint64_t>;

using Operation = std::tuple<Operand, Operator, Operand>;

struct Monkey {
    uint64_t id;
    std::queue<uint64_t> holding_items{};
    Operation op;
    uint64_t test;
    std::array<uint64_t, 2> throw_to;
    std::vector<Monkey>* monkey_list;
    uint64_t inspect_count{0};
    uint64_t biggest_divisor{0};

    Monkey(std::vector<Monkey>* ml, std::string input_str) : monkey_list(ml) {
        auto input_vec = AoC::split(input_str, '\n');
        id = std::stoull(
            AoC::split(AoC::split(input_vec[0], ':')[0], ' ').back());
        for (auto item : AoC::split(AoC::split(input_vec[1], ':')[1], ',')) {
            holding_items.push(std::stoull(item));
        }
        auto operation_str_list =
            AoC::split(AoC::split(input_vec[2], "=").back(), ' ');
        if (operation_str_list[0] != "old")
            std::get<0>(op) = std::stoull(operation_str_list[0]);
        switch (operation_str_list[1][0]) {
            case '+':
                std::get<1>(op) = Operator::plus;
                break;
            case '*':
                std::get<1>(op) = Operator::times;
                break;
            case '/':
                std::get<1>(op) = Operator::divided;
                break;
            case '-':
                std::get<1>(op) = Operator::minus;
                break;
            default:
                throw;
        }
        if (operation_str_list[2] != "old")
            std::get<2>(op) = std::stoull(operation_str_list[2]);
        test = std::stoull(AoC::split(input_vec[3], ' ').back());
        throw_to[0] = std::stoull(AoC::split(input_vec[5], ' ').back());
        throw_to[1] = std::stoull(AoC::split(input_vec[4], ' ').back());
    }

    uint64_t operation(uint64_t item) {
        uint64_t res;
        uint64_t lhs = std::get<0>(op).value_or(item);
        uint64_t rhs = std::get<2>(op).value_or(item);
        switch (std::get<1>(op)) {
            case Operator::plus:
                res = lhs + rhs;
                break;
            case Operator::divided:
                res = lhs / rhs;
                break;
            case Operator::times:
                res = lhs * rhs;
                break;
            case Operator::minus:
                res = lhs - rhs;
                break;
            default:
                throw;
        }
        return res % biggest_divisor;
    }

    void receive(uint64_t item) { holding_items.push(item); }

    void inspect() {
        auto item = holding_items.front();
        item = operation(item);
        auto dest = throw_to[!(item % test)];
        (*monkey_list)[dest].receive(item);
        holding_items.pop();
        inspect_count++;
    }

    void do_turn() {
        while (!holding_items.empty()) {
            inspect();
        }
    }
};

// 14106266886

int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    std::vector<Monkey> monkey_list;
    auto input = AoC::get_input(argv[1], "\n\n");
    uint64_t lcm = 1;
    for (auto monkey_str : input) {
        monkey_list.emplace_back(&monkey_list, monkey_str);
        lcm *= monkey_list.back().test;
    }
    for (auto& monkey : monkey_list) {
        monkey.biggest_divisor = lcm;
    }

    auto do_rounds = [&monkey_list](uint64_t n_of_rounds) {
        while (n_of_rounds--) {
            for (auto& monkey : monkey_list) {
                monkey.do_turn();
            }
        }
    };

    do_rounds(10000);

    std::sort(monkey_list.begin(), monkey_list.end(),
              [](const auto& a, const auto& b) {
                  return a.inspect_count > b.inspect_count;
              });
    std::cout << monkey_list[0].inspect_count * monkey_list[1].inspect_count
              << std::endl;
}
