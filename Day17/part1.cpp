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
using Point = AoC::Point<int>;

class Piece {
    std::vector<Point> m_squares;
    Point m_bl_corner;
    int m_height;

   public:
    Piece(const std::vector<Point>& m_squares)
        : m_squares(m_squares), m_bl_corner(0, 0) {
        int max_y = 0;
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
    constexpr static int m_width = 7;
    int highest_point = 0;
    int erased_lines = 0;
    std::vector<std::array<bool, m_width>> cave{{}, {}, {}};
    std::optional<Point> floating_piece_pos;
    std::optional<Piece> floating_piece;
    static auto get_next_piece() {
        const static std::array<Piece, 5> pieces = {
            Piece({{0, 0}, {1, 0}, {2, 0}, {3, 0}}),
            Piece({{0, 1}, {1, 0}, {1, 1}, {1, 2}, {2, 1}}),
            Piece({{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}}),
            Piece({{0, 0}, {0, 1}, {0, 2}, {0, 3}}),
            Piece({{0, 0}, {0, 1}, {1, 0}, {1, 1}})};
        static size_t i = 0;
        return pieces[i++ % pieces.size()];
    }

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
    void settle() {
        for (const auto& p : floating_piece->get_squares()) {
            const auto abs_pos = p + *floating_piece_pos;
            cave[abs_pos.y][abs_pos.x] = true;
        }
        highest_point = std::max(
            highest_point, floating_piece_pos->y + floating_piece->height());
        for (int size_diff = (highest_point + 3) - cave.size(); size_diff > 0;
             size_diff--)
            cave.push_back({});

        floating_piece_pos.reset();
        floating_piece.reset();

        uint8_t flood_detector{};
        for (size_t i = cave.size(); i > 0; i--) {
            for (size_t j = 0; j < m_width; j++)
                flood_detector |= cave[i - 1][j] << j;
            if (flood_detector == 0b01111111) {
                auto to_erase_cnt = i - 1;
                cave.erase(cave.begin(), cave.begin() + to_erase_cnt);
                highest_point -= to_erase_cnt;
                erased_lines += to_erase_cnt;
                break;
            }
        }
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
    }
    void wind(char direction_ch) {
        assert(floating_piece_pos.has_value() && floating_piece.has_value());
        if (const Point direction =
                (direction_ch == '>') ? Point{1, 0} : Point{-1, 0};
            can_move(direction))
            *floating_piece_pos += direction;
    }
    bool fall() {
        assert(floating_piece_pos.has_value() && floating_piece.has_value());
        if (const Point direction{0, -1}; can_move(direction)) {
            *floating_piece_pos += direction;
            return true;
        } else {
            settle();
            return false;
        }
    }
    auto max_height() const { return highest_point + erased_lines; }
    void print() const {
        for (int i = cave.size() - 1; i >= 0; i--) {
            std::cout << "|";
            for (int j = 0; j < m_width; j++) {
                if (cave[i][j])
                    std::cout << '#';
                else {
                    for (const auto& p : floating_piece->get_squares()) {
                        const auto abs_pos = p + *floating_piece_pos;
                        if (abs_pos.y == i && abs_pos.x == j) {
                            std::cout << '@';
                            goto next;
                        }
                    }
                    std::cout << '.';
                }
            next:;
            }
            std::cout << "|" << std::endl;
        }
        std::cout << "+-------+" << std::endl;
        std::cout << std::endl;
    }
};

// 3159
int main(int argc, char* argv[]) {
    // quick and dirty
    assert(argc == 2);
    const auto input = AoC::get_input(argv[1]);
    size_t input_idx = 0;
    Chamber c;
    for (auto cnt = 2022; cnt > 0; cnt--) {
        c.spawn();
        do {
            // c.print();
            c.wind(input[input_idx++ % input.size()]);
            // c.print();
        } while (c.fall());
    }
    std::cout << c.max_height() << std::endl;
    return 0;
}
