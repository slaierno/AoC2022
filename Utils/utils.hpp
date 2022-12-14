#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

namespace AoC {

template <typename T>
constexpr int signum(T val) {
    return (T(0) < val) - (val < T(0));
}

template <typename T = int>
struct Point {
    T x;
    T y;

    Point(auto x, auto y) : x(x), y(y) {}
    Point() {}
    void operator+=(const auto& oth) {
        x += oth.x;
        y += oth.y;
    }
    Point operator-(const auto& rhs) const { return {x - rhs.x, y - rhs.y}; }
    auto operator<=>(const Point&) const = default;
    T sup_norm() const { return std::max(std::abs(x), std::abs(y)); }
    Point unit_vec() { return {signum(x), signum(y)}; }
};

template <class T>
concept StringLike = std::is_convertible_v<T, std::string_view>;

constexpr auto profile = [](const auto& block,
                            const std::string_view message = "") {
    std::cout << message << std::endl;
    const auto start = std::chrono::steady_clock::now();
    const auto ret = block();
    const auto end = std::chrono::steady_clock::now();
    const std::chrono::duration<double> elapsed_seconds = end - start;
    std::cout << "elapsed time: " << elapsed_seconds.count() << "s"
              << std::endl;
    return ret;
};

auto idem_return = [](const std::string& s) -> std::string { return s; };

template <typename L = decltype(idem_return)>
auto split(std::string to_split,
           const std::initializer_list<std::string>& delimiters,
           L l = idem_return) {
    std::vector<decltype(l(""))> ret;
    for (size_t pos = 0; pos != std::string::npos;) {
        const auto it = std::min_element(
            delimiters.begin(), delimiters.end(), [&to_split](auto a, auto b) {
                return to_split.find(a) < to_split.find(b);
            });
        pos = to_split.find(*it);
        const auto token = to_split.substr(0, pos);
        if (token.length() > 0) ret.push_back(l(token));
        if (pos != std::string::npos) to_split.erase(0, pos + it->size());
    }
    return ret;
}

template <typename L = decltype(idem_return)>
auto split(std::string to_split, const std::string& delimiter,
           L l = idem_return) {
    return AoC::split(to_split, {delimiter}, l);
}

template <typename L = decltype(idem_return)>
auto split(std::string to_split, const char delimiter, L l = idem_return) {
    return AoC::split(to_split, std::string(1, delimiter), l);
}

auto get_input(const std::string& filename) {
    if (std::ifstream input_file(filename); input_file.is_open()) {
        std::stringstream buffer;
        buffer << input_file.rdbuf();
        return buffer.str();
    } else
        throw "Invalid input\n";
};

template <typename L = decltype(idem_return)>
auto get_input(const std::string& filename,
               const std::initializer_list<std::string>& delimiters,
               L l = idem_return) {
    if (std::ifstream input_file(filename); input_file.is_open()) {
        std::stringstream buffer;
        buffer << input_file.rdbuf();
        return AoC::split(buffer.str(), delimiters, l);
    } else
        throw "Invalid input\n";
};

template <typename L = decltype(idem_return)>
auto get_input(const std::string& filename, const std::string& delimiter,
               L l = idem_return) {
    return AoC::get_input(filename, {delimiter}, l);
}

template <typename L = decltype(idem_return)>
auto get_input(const std::string& filename, const char delimiter,
               L l = idem_return) {
    return AoC::get_input(filename, std::string(1, delimiter), l);
};

template <template <typename> typename T = std::vector>
constexpr auto for_each_ret(const auto& b, const auto& e, const auto& l) {
    T<decltype(l(*b))> ret;
    std::for_each(b, e, [l, &ret](auto el) { ret.emplace_back(l(el)); });
    return ret;
}
}  // namespace AoC
