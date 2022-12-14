#include <cassert>
#include <charconv>
#include <fstream>
#include <iostream>
#include <memory>
#include <range/v3/all.hpp>
#include <variant>

#include "utils.hpp"
using namespace ranges;

class IntOrList {
    using List = std::vector<std::shared_ptr<IntOrList>>;
    std::variant<std::monostate, int, List> m_data;

   public:
    IntOrList() {}
    IntOrList(int i) : m_data(i) {}
    IntOrList(std::string_view& sv) {
        if (sv[0] != '[')
            add_int(sv);
        else
            add_list(sv = sv.substr(1));
    }
    void add_int(std::string_view& sv) {
        auto end_p = sv.find_first_not_of("0123456789");
        int i = 0;
        if (std::errc::invalid_argument ==
            std::from_chars(sv.data(), sv.data() + end_p, i).ec)
            throw;
        m_data = i;
        sv = sv.substr(end_p);
    }
    void add_list(std::string_view& sv) {
        m_data = List{};
        while (sv[0] != ']') {
            std::get<2>(m_data).emplace_back(std::make_shared<IntOrList>(sv));
            if (sv[0] == ',') sv = sv.substr(1);
        }
        sv = sv.substr(1);
    }

    std::strong_ordering compare_lists(const List& l1, const List& l2) const {
        const size_t min_size = std::min(l1.size(), l2.size());
        for (size_t i = 0; i < min_size; i++)
            if (auto comp = *l1[i] <=> *l2[i]; comp != 0) return comp;
        return l1.size() <=> l2.size();
    }
    std::strong_ordering operator<=>(const IntOrList& oth) const {
        assert(m_data.index() != 0 && oth.m_data.index() != 0);
        if (m_data.index() == oth.m_data.index())
            if (m_data.index() == 1)
                return std::get<1>(m_data) <=> std::get<1>(oth.m_data);
            else
                return compare_lists(std::get<2>(m_data),
                                     std::get<2>(oth.m_data));
        else if (m_data.index() == 1)
            return get_single_list(*this) <=> oth;
        else
            return *this <=> get_single_list(oth);
    }

    static IntOrList get_single_list(auto val) {
        auto iol = IntOrList();
        iol.m_data = List{std::make_shared<IntOrList>(val)};
        return iol;
    }
};

// 6478
int main(int argc, [[maybe_unused]] char* argv[]) {
    // quick and dirty
    assert(argc == 2);

    const auto input = AoC::get_input(argv[1], "\n\n");
    size_t cnt = 0;
    for (size_t i = 0; i < input.size(); i++) {
        const auto& pair = input[i];
        auto sep = pair.find_first_of("\n");
        auto sv1 = std::string_view(pair).substr(0, sep);
        auto sv2 = std::string_view(pair).substr(sep + 1);
        if (IntOrList(sv1) < IntOrList(sv2)) cnt += (i + 1);
    }
    std::cout << cnt << std::endl;
    return 0;
}
