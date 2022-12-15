#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
#include <unordered_set>

#include "utils.hpp"

using namespace ranges;
using Point = AoC::Point<int>;
using SensorBeacon = std::pair<Point, Point>;

constexpr Point RX{1, 0};

constexpr int target_y = 2000000;

// 5256611
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    const auto input = AoC::get_input(argv[1], "\n", [](auto v) {
        const auto split = AoC::split(v, " ");
        Point sensor(split[2].substr(2, split[2].size() - 3) + "," +
                     split[3].substr(2, split[3].size() - 3));
        Point beacon(split[8].substr(2, split[8].size() - 3) + "," +
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
    std::unordered_set<Point> empty_list;
    for (const auto& [s, d] : sensor_list) {
        const Point start = {s.x, target_y};
        auto ddiff = d - Point::taxi_dist(start, s);
        for (auto exploring = start - Point{ddiff, 0};
             exploring.x <= start.x + ddiff; exploring += RX)
            if (!beacon_list.contains(exploring)) empty_list.insert(exploring);
    }
    std::cout << empty_list.size() << std::endl;
    return 0;
}
