// #include <iostream>
// #include <fstream>
// #include <cassert>
// #include <range/v3/all.hpp>
// #include "utils.hpp"

// class RPS {
//     int m_rps;
// public:
//     constexpr RPS(char c) : m_rps(c - (c < 'X' ? 'A' : 'X')) {}
//     constexpr auto operator<=>(const RPS& rhs) const {
//         if (m_rps == rhs.m_rps) return 0;
//         else if ((m_rps + 1) % 3 == rhs.m_rps) return -1;
//         else return 1;
//     }
//     constexpr auto score() const { return m_rps + 1; }
// };

// uint64_t get_round_score(const std::pair<RPS, RPS>& round) {
//     return 3*((round.second <=> round.first) + 1) + round.second.score();
// }

// //13526
// int main(int argc, char *argv[])
// {
//     using namespace ranges;

//     //quick and dirty
//     assert(argc == 2);

//     auto score = accumulate( 
//         AoC::get_input(argv[1], "\n", [](auto v){
//             return get_round_score({v[0], v[2]});
//         }),
//         0ULL);

//     std::cout << score << std::endl;

//     return 0;
// }
