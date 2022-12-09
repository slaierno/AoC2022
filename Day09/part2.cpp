#include <array>
#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
// #include <ranges>
#include <set>
#include <string>

#include "utils.hpp"

template <typename T>
int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

struct Point {
    int x;
    int y;

    void operator+=(auto oth) {
        x += oth.x;
        y += oth.y;
    }
    Point operator-(const auto& rhs) const { return {x - rhs.x, y - rhs.y}; }
    auto operator<=>(const Point&) const = default;
    int sup_norm() const { return std::max(std::abs(x), std::abs(y)); }
    static Point unit_vec(const auto& p) { return {sgn(p.x), sgn(p.y)}; }
};

struct Instruction {
    Point direction;
    unsigned steps;

    Instruction(const std::string& str)
        : direction(dir_to_point(str[0])), steps(std::stoull(str.substr(2))) {}

    static Point dir_to_point(char c) {
        switch (c) {
            case 'U':
                return {0, 1};
            case 'D':
                return {0, -1};
            case 'L':
                return {-1, 0};
            case 'R':
                return {1, 0};
            default:
                throw;
        }
    }
};

// 2487
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    const auto input = AoC::get_input(
        argv[1], '\n', [](const auto& v) { return Instruction(v); });

    std::array<Point, 10> knots{0, 0};
    Point& H = knots.front();
    Point& T = knots.back();

    std::set<Point> tail_cover_map{T};

    for (const auto& [d, s] : input) {
        for (int steps = s; steps--;) {
            H += d;
            for (const auto& knot_pair :
                 views::all(knots) | views::sliding(2)) {
                if (auto diff = knot_pair[0] - knot_pair[1];
                    diff.sup_norm() > 1) {
                    knot_pair[1] += Point::unit_vec(diff);
                }
            }
            tail_cover_map.insert(T);
        }
    }

    std::cout << tail_cover_map.size() << std::endl;
    return 0;
}
