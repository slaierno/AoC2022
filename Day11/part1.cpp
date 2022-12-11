#include <cassert>
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

using Operand = std::optional<unsigned>;

using Operation = std::tuple<Operand, Operator, Operand>;

struct Monkey {
    unsigned id;
    std::queue<unsigned> holding_items{};
    Operation op;
    unsigned test;
    std::array<unsigned, 2> throw_to;
    std::vector<Monkey>* monkey_list;
    unsigned inspect_count{0};

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

    unsigned operation(unsigned item) {
        unsigned lhs = std::get<0>(op).value_or(item);
        unsigned rhs = std::get<2>(op).value_or(item);
        switch (std::get<1>(op)) {
            case Operator::plus:
                return lhs + rhs;
                break;
            case Operator::divided:
                return lhs / rhs;
                break;
            case Operator::times:
                return lhs * rhs;
                break;
            case Operator::minus:
                return lhs - rhs;
                break;
            default:
                throw;
        }
    }

    void receive(unsigned item) { holding_items.push(item); }

    void inspect() {
        auto item = holding_items.front();
        item = operation(item);
        item = item / 3;
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

// 57348
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    std::vector<Monkey> monkey_list;
    auto input = AoC::get_input(argv[1], "\n\n");
    for (auto monkey_str : input) {
        monkey_list.emplace_back(&monkey_list, monkey_str);
    }

    auto do_rounds = [&monkey_list](unsigned n_of_rounds) {
        while (n_of_rounds--) {
            for (auto& monkey : monkey_list) {
                monkey.do_turn();
            }
        }
    };

    do_rounds(20);

    std::sort(monkey_list.begin(), monkey_list.end(),
              [](const auto& a, const auto& b) {
                  return a.inspect_count > b.inspect_count;
              });
    std::cout << monkey_list[0].inspect_count * monkey_list[1].inspect_count
              << std::endl;
}
