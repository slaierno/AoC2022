#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <range/v3/all.hpp>
#include <set>

#include "utils.hpp"
using namespace ranges;

auto dijkstra(const char* start, const char* end, const auto& graph,
              const auto& input) {
    std::unordered_map<const char*, size_t> dist;
    std::unordered_map<const char*, const char*> prev;

    auto priority = [&dist](const char* a, const char* b) {
        return (dist[a] < dist[b]) ? true : a < b;
    };
    std::set<const char*, decltype(priority)> Q(priority);

    for (const auto& line : input) {
        for (const auto& ch : line) {
            dist[&ch] = ULLONG_MAX;
            prev[&ch] = nullptr;
            Q.insert(&ch);
        }
    }
    dist[start] = 0;
    prev[start] = nullptr;
    Q.insert(start);

    while (!Q.empty()) {
        const auto u = Q.extract(Q.begin()).value();
        for (const auto& v : graph.at(u)) {
            auto dist_v_u = (v == start) ? 0 : 1;
            if (auto alt = dist[u] + dist_v_u; alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                if (auto nv = Q.extract(v); !nv.empty())
                    Q.insert(std::move(nv));
                else
                    Q.insert(v);
            }
        }
    }

    unsigned long long path_length = 0;
    for (auto node = end; prev[node] != start; node = prev[node]) path_length++;

    return path_length;
}

// 332
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    const char* end = nullptr;
    char start = 'a';  // dummy start
    std::unordered_map<const char*, std::vector<const char*>> graph;
    std::vector<std::string> input = AoC::get_input(argv[1], "\n");

    // prerun to find start and end
    for (size_t i = 0; i < input.size(); i++) {
        auto& line = input[i];
        for (size_t j = 0; j < line.size(); j++) {
            auto& curr = line[j];
            switch (curr) {
                case 'S':
                    curr = 'a';
                    break;
                case 'E':
                    end = &curr;
                    curr = 'z';
                    break;
                default:
                    break;
            }
        }
    }

    // make the graph
    graph[&start] = {};
    for (size_t i = 0; i < input.size(); i++) {
        auto& line = input[i];
        for (size_t j = 0; j < line.size(); j++) {
            auto& curr = line[j];
            graph[&curr] = {};  // empty init
            if (curr == 'a') {
                graph[&curr].push_back(&start);
                graph[&start].push_back(&curr);
            }
            for (size_t i_n = (i ? i - 1 : i);
                 i_n <= i + 1 && i_n < input.size(); i_n++) {
                for (size_t j_n = (j ? j - 1 : j);
                     j_n <= j + 1 && j_n < line.size(); j_n++) {
                    if ((i_n == i) == (j_n == j)) continue;
                    auto& neigh = input[i_n][j_n];
                    if (neigh <= curr + 1)  //
                        graph[&curr].push_back(&neigh);
                }
            }
            if (graph.find(&curr) == graph.end()) {
                std::cout << i << "," << j << std::endl;
            }
        }
    }

    assert(graph.size() == 1 + input.size() * input.back().size());
    assert(end);
    unsigned long long min_path_length = dijkstra(&start, end, graph, input);

    std::cout << min_path_length << std::endl;

    return 0;
}
