#include <z3++.h>

#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>

#include "utils.hpp"

using namespace z3;
using namespace ranges;
using Point = AoC::Point<int>;

constexpr int max_dist = 4000000;

// 13337919186981
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    context c;
    expr x = c.int_const("x");
    expr y = c.int_const("y");
    solver solv(c);

    auto taxi = [](auto x, auto y, auto x1, auto y1) {
        auto abs = [](auto x) { return ite(x >= 0, x, -x); };
        return abs(x - x1) + abs(y - y1);
    };

    solv.add(0 <= x && x <= max_dist);
    solv.add(0 <= y && y <= max_dist);

    const auto input = AoC::get_input(argv[1]);
    for (auto line : input | views::split('\n')) {
        const auto split = line | to<std::string>() | actions::split(' ');
        const Point s(std::stol(split[2].substr(2, split[2].size() - 3)),
                      std::stol(split[3].substr(2, split[3].size() - 3)));
        const Point b(std::stol(split[8].substr(2, split[8].size() - 3)),
                      std::stol(split[9].substr(2, split[9].size() - 2)));
        solv.add(taxi(x, y, s.x, s.y) > Point::taxi_dist(s, b));
    }

    solv.check();
    auto m = solv.get_model();
    std::cout << m.eval(x).as_int64() * 4000000 + m.eval(y).as_int64()
              << std::endl;
    return 0;
}
