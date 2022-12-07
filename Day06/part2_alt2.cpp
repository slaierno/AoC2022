#include <cassert>
#include <fstream>
#include <iostream>
#include <range/v3/all.hpp>
#include <unordered_map>

#include "utils.hpp"

using namespace ranges;

class SetCount {
    std::unordered_map<char, unsigned> m_countmap;

   public:
    SetCount(const auto& it_begin, const auto& it_end) {
        for_each(it_begin, it_end, [this](const auto c) { insert(c); });
    }
    void insert(const char c) { m_countmap[c]++; }
    void erase(const char c) {
        if (auto search = m_countmap.find(c); --search->second == 0)
            m_countmap.erase(search);
    }
    auto size() const { return m_countmap.size(); }
};

// 3986
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    constexpr size_t marker_size = 14;
    const auto input = AoC::get_input(argv[1]);

    SetCount count_map(input.begin(), input.begin() + marker_size - 1);

    for (size_t i = 0; i < input.size() - marker_size + 1; i++) {
        count_map.insert(input[i + marker_size - 1]);
        if (count_map.size() == marker_size) {
            std::cout << i + marker_size << std::endl;
            return 0;
        }
        count_map.erase(input[i]);
    }

    std::cout << "ERROR: no marker found!" << std::endl;
    return -1;
}
