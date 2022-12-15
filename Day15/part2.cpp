#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
#include <unordered_set>

#include "utils.hpp"

using namespace ranges;
using Point = AoC::Point<int>;
using SensorBeacon = std::pair<Point, Point>;

constexpr int max_dist = 4000000;

// 13337919186981
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    const auto input = AoC::get_input(argv[1], "\n", [](auto v) {
        const auto split = AoC::split(v, " ");
        const Point sensor(split[2].substr(2, split[2].size() - 3) + "," +
                           split[3].substr(2, split[3].size() - 3));
        const Point beacon(split[8].substr(2, split[8].size() - 3) + "," +
                           split[9].substr(2, split[9].size() - 2));
        return SensorBeacon{sensor, beacon};
    });

    const auto [sensor_list, beacon_list] = ({
        std::vector<std::pair<Point, int>> sensor_list;
        std::unordered_set<Point> beacon_list;
        for (const auto& [s, b] : input) {
            sensor_list.emplace_back(s, Point::taxi_dist(s, b));
            beacon_list.insert(b);
        }
        std::pair{sensor_list, beacon_list};
    });

#pragma omp parallel for
    for (int y = 0; y <= max_dist; y++) {
        const auto cover_ranges = ({
            std::vector<std::pair<int, int>> cover_ranges;
            for (const auto& [s, d] : sensor_list)
                if (auto ddiff = d - Point::taxi_dist({s.x, y}, s); ddiff >= 0)
                    cover_ranges.emplace_back(s.x - ddiff, s.x + ddiff);
            sort(cover_ranges, [](const auto& a, const auto& b) {
                return (a.first != b.first) ? a.first < b.first
                                            : a.second < b.second;
            });
            cover_ranges;
        });
        for (int x = 0; x < max_dist; x++) {
            bool search = false;
            for (const auto& r : cover_ranges) {
                if (x >= r.first && x <= r.second) {
                    x = std::max(x, r.second);
                    search = true;
                }
            }
            if (!search && !beacon_list.contains(Point(x, y))) {
                std::cout << (int64_t)x * 4000000 + y << std::endl;
                break;
            }
        }
    }
    return 0;
}
