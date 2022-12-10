#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <range/v3/all.hpp>
#include <string_view>

#include "utils.hpp"
using namespace ranges;

struct Instruction {
    enum {
        noop = 0,
        addx,
    } op;
    int value;
    Instruction() = default;
    Instruction(const std::string& str) {
        const auto str_split = AoC::split(str, " ");
        const auto& op_str = str_split[0];
        if (op_str == "noop") {
            op = noop;
        } else if (op_str == "addx") {
            op = addx;
            value = std::stoull(str_split[1]);
        }
    }
};

struct CPU {
    int X{1};
    uint64_t clock_cnt{1};
    std::vector<Instruction> inst_list;
    unsigned inst_ptr{0};
    // void noop(int val) {}
    void addx(int val) { X += val; }
    unsigned wait_cnt{0};
    decltype(Instruction::op) next_op{Instruction::noop};
    int next_val{0};
    bool halt{false};
    void fetch() {
        const auto& [op, val] = inst_list[inst_ptr++];
        next_op = op;
        switch (op) {
            case Instruction::noop:
                wait_cnt = 1;
                break;
            case Instruction::addx:
                wait_cnt = 2;
                next_val = val;
                break;
            default:
                throw;
        }
    }
    void exec() {
        switch (next_op) {
            case Instruction::noop:
                break;
            case Instruction::addx:
                addx(next_val);
                break;
            default:
                throw;
        }
    }
    void print() {
        int draw_position = (clock_cnt - 1) % 40;
        if (draw_position == 0) std::cout << "\n";
        if (draw_position >= X - 1 && draw_position <= X + 1) {
            std::cout << "#";
        } else {
            std::cout << ".";
        }
    }
    void tick() {
        if (inst_ptr == inst_list.size()) {
            halt = true;
            return;
        }
        print();
        clock_cnt++;
        // fetch
        if (0 == wait_cnt) fetch();
        // exec
        wait_cnt--;
        if (0 == wait_cnt) exec();
    }
    auto signal_strength() const { return X * (clock_cnt); }
    void print_state() const {
        std::cout << "Clock: " << clock_cnt << "; X: " << X
                  << "; SS: " << signal_strength() << std::endl;
    }
};

// BPJAZGAP
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    CPU cpu{.inst_list = AoC::get_input(
                argv[1], '\n', [](const auto& v) { return Instruction(v); })};

    while (!cpu.halt) cpu.tick();
    return 0;
}
