#include <cassert>
#include <deque>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>

#include "utils.hpp"

// 1300
int main(int argc, char* argv[]) {
    using namespace ranges;

    // quick and dirty
    assert(argc == 2);

    std::deque<char> marker_detector{};
    constexpr size_t marker_size = 4;
    const auto input = AoC::get_input(argv[1]);
    for (const auto [i, c] : views::enumerate(input)) {
        if (marker_detector.size() == marker_size) marker_detector.pop_front();
        if (!marker_detector.empty()) {
            if (const auto f = find(marker_detector, c);
                f != marker_detector.end()) {
                marker_detector.erase(marker_detector.begin(), f + 1);
            } else if (marker_detector.size() == marker_size - 1) {
                std::cout << i + 1 << std::endl;
                return 0;
            }
        }
        marker_detector.push_back(c);
    }
    std::cout << "ERROR: no marker found!" << std::endl;
    return -1;
}
