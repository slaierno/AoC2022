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

constexpr unsigned start_time = 26;

class ValveList_t {
    std::set<Node> m_data;

   public:
    auto insert(const auto& n) { return m_data.insert(n); }
    auto contains(const auto& n) const { return m_data.contains(n); }
    auto size() const { return m_data.size(); }
    std::string to_string() const { return accumulate(m_data, std::string{}); }
    auto opening(Node n) const {
        auto copy = *this;
        copy.insert(n);
        return copy;
    }
    auto opening(Node n1, Node n2) const {
        auto copy = *this;
        copy.insert(n1);
        copy.insert(n2);
        return copy;
    }
};

using Memoization_t = std::unordered_map<
    Node,
    std::unordered_map<unsigned, std::unordered_map<std::string, unsigned>>>;

unsigned explore(const Node from_you, const Node from_ele, unsigned time,
                 const ValveList_t opened_valves, Memoization_t& memo,
                 const Graph& g) {
    if (time == 1) return 0;
    if (opened_valves.size() == g.pressure_rate.size()) return 0;
    {
        const auto from =
            (from_you < from_ele) ? from_you + from_ele : from_ele + from_you;
        if (memo.contains(from) && memo[from].contains(time) &&
            memo[from][time].contains(opened_valves.to_string())) {
            return memo[from][time][opened_valves.to_string()];
        }
    }
    unsigned score = 0;
    auto should_open = [&g, &opened_valves](const Node n) {
        return g.pressure_rate.at(n) > 0 && !opened_valves.contains(n);
    };
    // I explore next node
    for (auto to_you : g.edges.at(from_you)) {
        // Elephant explores next node
        for (auto to_ele : g.edges.at(from_ele)) {
            score = std::max(score, explore(to_you, to_ele, time - 1,
                                            opened_valves, memo, g));
        }
        if (should_open(from_ele)) {
            // Elephant opens valve
            const unsigned opened_score =
                (time - 1) * g.pressure_rate.at(from_ele);
            score = std::max(
                score, opened_score + explore(to_you, from_ele, time - 1,
                                              opened_valves.opening(from_ele),
                                              memo, g));
        }
    }
    // I open valve
    if (should_open(from_you)) {
        const unsigned you_opened_score =
            (time - 1) * g.pressure_rate.at(from_you);
        // Elephant explores next node
        for (auto to_ele : g.edges.at(from_ele)) {
            score = std::max(
                score, you_opened_score +
                           explore(from_you, to_ele, time - 1,
                                   opened_valves.opening(from_you), memo, g));
        }
        if (from_you != from_ele && should_open(from_ele)) {
            const unsigned ele_opened_score =
                (time - 1) * g.pressure_rate.at(from_ele);
            score = std::max(
                score, you_opened_score + ele_opened_score +
                           explore(from_you, from_ele, time - 1,
                                   opened_valves.opening(from_ele, from_you),
                                   memo, g));
        }
    }
    {
        const auto from =
            (from_you < from_ele) ? from_you + from_ele : from_ele + from_you;
        memo[from][time][opened_valves.to_string()] = score;
    }
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
    std::cout << explore("AA", "AA", start_time, opened_valves, memo, g)
              << std::endl;
    return 0;
}
