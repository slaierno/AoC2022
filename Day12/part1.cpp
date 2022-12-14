#include <cassert>
#include <fstream>
#include <iostream>
#include <limits>
#include <range/v3/all.hpp>
#include <set>

#include "utils.hpp"
using namespace ranges;

// 339
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    const char *start, *end;
    std::unordered_map<const char*, std::vector<const char*>> graph;
    std::unordered_map<const char*, size_t> dist;
    std::unordered_map<const char*, const char*> prev;
    std::vector<std::string> input = AoC::get_input(argv[1], "\n");

    // prerun to find start and end
    for (size_t i = 0; i < input.size(); i++) {
        auto& line = input[i];
        for (size_t j = 0; j < line.size(); j++) {
            auto& curr = line[j];
            switch (curr) {
                case 'S':
                    start = &curr;
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
    for (size_t i = 0; i < input.size(); i++) {
        auto& line = input[i];
        for (size_t j = 0; j < line.size(); j++) {
            auto& curr = line[j];
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
        }
    }

    auto priority = [&dist](const char* a, const char* b) {
        return (dist[a] < dist[b]) ? true : a < b;
    };
    std::set<const char*, decltype(priority)> Q(priority);

    dist[start] = 0;
    prev[start] = nullptr;
    for (const auto& line : input) {
        for (const auto& ch : line) {
            if (&ch != start) {
                dist[&ch] = ULLONG_MAX;
                prev[&ch] = nullptr;
            }
            Q.insert(&ch);
        }
    }
    while (!Q.empty()) {
        const auto u = Q.extract(Q.begin()).value();
        for (const auto& v : graph[u]) {
            if (auto alt = dist[u] + 1; alt < dist[v]) {
                dist[v] = alt;
                prev[v] = u;
                if (auto nv = Q.extract(v); !nv.empty())
                    Q.insert(std::move(nv));
                else
                    Q.insert(v);
            }
        }
    }
    unsigned path_length = 0;
    for (auto node = end; node != start; path_length++, node = prev[node])
        ;
    std::cout << path_length << std::endl;
    return 0;
}
