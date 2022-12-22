#include <cassert>
#include <cmath>
#include <fstream>
#include <iostream>
#include <map>
#include <optional>
#include <range/v3/all.hpp>
#include <string>
#include <vector>

#include "utils.hpp"
using namespace ranges;
using Point = AoC::Point<int64_t>;

constexpr uint64_t nof_pieces = 1000000000000ULL;

class Piece {
    std::vector<Point> m_squares;
    Point m_bl_corner;
    int64_t m_height;

   public:
    Piece(const std::vector<Point>& m_squares)
        : m_squares(m_squares), m_bl_corner(0, 0) {
        int64_t max_y = 0;
        for (const auto& p : m_squares) {
            m_bl_corner.x = std::min(m_bl_corner.x, p.x);
            m_bl_corner.y = std::min(m_bl_corner.y, p.y);
            max_y = std::max(max_y, p.y);
        }
        m_height = std::abs(max_y - m_bl_corner.y) + 1;
    }
    Point get_bottom_left_corner() const { return m_bl_corner; };
    const auto& get_squares() const { return m_squares; }
    const auto& height() const { return m_height; }
};

class Chamber {
    constexpr static int64_t m_width = 7;
    int64_t highest_point = 0;
    int64_t erased_lines = 0;
    std::vector<std::array<bool, m_width>> cave{{}, {}, {}};
    std::optional<Point> floating_piece_pos;
    std::optional<Piece> floating_piece;
    using flood_detector_t = std::array<int64_t, m_width>;
    using cycle_key_t = std::tuple<flood_detector_t, size_t, size_t>;
    using idx_height_pair_t = std::pair<uint64_t, uint64_t>;
    std::map<cycle_key_t, idx_height_pair_t> cycle_map;
    size_t piece_idx = 0;
    const std::array<Piece, 5> pieces = {
        Piece({{0, 0}, {1, 0}, {2, 0}, {3, 0}}),
        Piece({{0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1}}),
        Piece({{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}}),
        Piece({{0, 0}, {0, 1}, {0, 2}, {0, 3}}),
        Piece({{0, 0}, {0, 1}, {1, 0}, {1, 1}})};
    auto get_next_piece() { return pieces[piece_idx++ % pieces.size()]; }

    bool is_free(const Point& pos) const {
        return pos.x >= 0 && pos.x < m_width && pos.y >= 0 &&
               !cave[pos.y][pos.x];
    }
    bool can_move(const Point& direction) const {
        assert(floating_piece_pos.has_value() && floating_piece.has_value());
        return all_of(floating_piece->get_squares(),
                      [this, &direction](const auto& p) {
                          return is_free(p + *floating_piece_pos + direction);
                      });
    }
    void settle(const size_t dir_rel_idx, size_t& fallen_pieces) {
        static bool cycle_detected = false;
        for (const auto& p : floating_piece->get_squares()) {
            const auto abs_pos = p + *floating_piece_pos;
            cave[abs_pos.y][abs_pos.x] = true;
        }
        highest_point = std::max(
            highest_point, floating_piece_pos->y + floating_piece->height());
        for (int64_t size_diff = (highest_point + 3) - cave.size();
             size_diff > 0; size_diff--)
            cave.push_back({});

        floating_piece_pos.reset();
        floating_piece.reset();

        flood_detector_t flood_detector;
        flood_detector.fill(-1);
        for (size_t j = 0; j < flood_detector.size(); j++) {
            for (size_t i = cave.size(); i > 0; i--) {
                if (cave[i - 1][j]) {
                    flood_detector[j] = i - 1;
                    break;
                }
            }
        }
        const auto keep_from = *min_element(flood_detector);
        if (keep_from >= 0) {
            if (keep_from > 0) {
                cave.erase(cave.begin(), cave.begin() + keep_from);
                highest_point -= keep_from;
                erased_lines += keep_from;
                flood_detector |= actions::transform(
                    [keep_from](const auto x) { return x - keep_from; });
            }
            if (!cycle_detected) {
                const auto flood_detector_tuple = cycle_key_t(
                    flood_detector, dir_rel_idx, fallen_pieces % pieces.size());
                if (cycle_map.contains(flood_detector_tuple)) {
                    cycle_detected = true;
                    const auto& [cycle_piece_idx, cycle_height] =
                        cycle_map.at(flood_detector_tuple);
                    const auto piece_cycle_size =
                        fallen_pieces - cycle_piece_idx;
                    const auto nof_cycles =
                        (nof_pieces - (fallen_pieces + 1)) / piece_cycle_size;
                    piece_idx += nof_cycles * piece_cycle_size;
                    fallen_pieces += nof_cycles * piece_cycle_size;
                    erased_lines +=
                        (highest_point + erased_lines - cycle_height) *
                        nof_cycles;
                } else {
                    cycle_map[flood_detector_tuple] = idx_height_pair_t(
                        fallen_pieces, highest_point + erased_lines);
                }
            }
        }
        // uint8_t flood_detector = 0;
        // for (size_t i = cave.size(); i > 0; i--) {
        //     for (size_t j = 0; j < m_width; j++)
        //         flood_detector |= cave[i - 1][j] << j;
        //     if (flood_detector == 0b01111111) {
        //         auto to_erase_cnt = i - 1;
        //         cave.erase(cave.begin(), cave.begin() + to_erase_cnt);
        //         highest_point -= to_erase_cnt;
        //         erased_lines += to_erase_cnt;
        //         break;
        //     }
        // }
    }

   public:
    // TODO ensure that cave.size() (i.e. cave height) is always as big as
    // highest point + 3 + new piece heigth
    void spawn() {
        assert(!floating_piece_pos.has_value() && !floating_piece.has_value());
        auto p = get_next_piece();
        floating_piece_pos =
            p.get_bottom_left_corner() + Point{2, highest_point + 3};
        floating_piece = p;
        for (auto h = floating_piece->height(); h--;) cave.push_back({});
        print();
    }
    void wind(char direction_ch) {
        assert(floating_piece_pos.has_value() && floating_piece.has_value());
        if (const Point direction =
                (direction_ch == '>') ? Point{1, 0} : Point{-1, 0};
            can_move(direction))
            *floating_piece_pos += direction;
    }
    bool fall(const size_t dir_rel_idx, size_t& fallen_pieces) {
        assert(floating_piece_pos.has_value() && floating_piece.has_value());
        if (const Point direction{0, -1}; can_move(direction)) {
            *floating_piece_pos += direction;
            return true;
        } else {
            settle(dir_rel_idx, fallen_pieces);
            return false;
        }
    }
    auto max_height() const { return highest_point + erased_lines; }
    void print() const {
        std::string outstr = "";
        for (int64_t i = cave.size() - 1; i >= 0; i--) {
            outstr += "|";
            for (int64_t j = 0; j < m_width; j++) {
                outstr +=
                    any_of(floating_piece->get_squares(),
                           [this, i, j](const auto& p) {
                               return (p + *floating_piece_pos) == Point(j, i);
                           })
                        ? '@'
                    : cave[i][j] ? '#'
                                 : '.';
            }
            outstr += "|\n";
        }
        outstr += "+-------+\n";
        std::cout << outstr << std::endl;
    }
};

// 1566272189352
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);
    const auto input = AoC::get_input(argv[1]);
    size_t input_idx = 0;
    Chamber c;
    for (size_t piece_no = 0; piece_no < nof_pieces; piece_no++) {
        c.spawn();
        do c.wind(input[input_idx % input.size()]);
        while (c.fall(input_idx++ % input.size(),
                      piece_no));  // fall can modify piece_no
    }
    std::cout << c.max_height() << std::endl;
    return 0;
}
