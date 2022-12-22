#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <range/v3/all.hpp>
#include <set>
#include <unordered_set>

#include "utils.hpp"
using namespace ranges;

using Node = std::string;
struct Graph {
    std::unordered_map<Node, std::vector<Node>> edges;
    std::unordered_map<Node, unsigned> pressure_rate;

    // std::unordered_map<Node, std::pair<Node>> a;

    void add(const std::string& sv) {
        const auto splits = AoC::split(sv, ' ');
        const auto node = splits[1];
        pressure_rate[node] =
            std::stoull(AoC::split(AoC::split(splits[4], '=')[1], ';')[0]);
        for (size_t i = 9; i < splits.size(); i++) {
            edges[node].emplace_back(AoC::split(splits[i], ',')[0]);
        }
    }
};

constexpr unsigned start_time = 30;

class ValveList_t {
    std::set<Node> m_data;

   public:
    auto insert(const auto& n) { return m_data.insert(n); }
    auto contains(const auto& n) const { return m_data.contains(n); }
    auto size() const { return m_data.size(); }
    std::string to_string() const { return accumulate(m_data, std::string{}); }
};

using Memoization_t = std::unordered_map<
    Node,
    std::unordered_map<unsigned, std::unordered_map<std::string, unsigned>>>;

unsigned explore(Node from, unsigned time, const ValveList_t opened_valves,
                 Memoization_t& memo, const Graph& g) {
    if (time == 1) return 0;
    if (opened_valves.size() == g.pressure_rate.size()) return 0;
    if (memo.contains(from) && memo[from].contains(time) &&
        memo[from][time].contains(opened_valves.to_string())) {
        return memo[from][time][opened_valves.to_string()];
    }
    unsigned score = 0;
    for (auto to : g.edges.at(from)) {
        score = std::max(score, explore(to, time - 1, opened_valves, memo, g));
    }
    if (g.pressure_rate.at(from) > 0 && !opened_valves.contains(from)) {
        const unsigned opened_score = (time - 1) * g.pressure_rate.at(from);
        auto copy_opened_valves = opened_valves;
        copy_opened_valves.insert(from);
        score = std::max(
            score, opened_score +
                       explore(from, time - 1, copy_opened_valves, memo, g));
    }
    memo[from][time][opened_valves.to_string()] = score;
    return score;
}
// 339
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    std::vector<std::string> input = AoC::get_input(argv[1], "\n");
    Graph g{};
    for (auto line : input) {
        g.add(line);
    }
    Memoization_t memo{};
    ValveList_t opened_valves{};
    std::cout << explore("AA", start_time, opened_valves, memo, g) << std::endl;
    return 0;
}
