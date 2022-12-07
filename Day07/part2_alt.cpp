#include <cassert>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <range/v3/all.hpp>
#include <set>
#include <string_view>

#include "utils.hpp"

using namespace ranges;

class Folder {
    const std::string name;
    Folder* const parent;
    std::map<std::string, size_t> files;
    std::map<std::string, std::shared_ptr<Folder>, std::less<>> folders;
    size_t size{0};
    size_t total_size{0};
    bool total_size_setted{false};

   public:
    Folder(auto name, auto parent) : name(name), parent(parent) {}
    void add_file(size_t fsize, const std::string& fname) {
        if (files.end() == files.find(fname)) {
            size += fsize;
            files.insert({fname, fsize});
        }
    }
    void add_folder(const std::string& dirname) {
        folders.insert({dirname, std::make_shared<Folder>(dirname, this)});
    }
    auto get_parent() const { return parent; }
    auto get_folder(std::string_view dirname) const {
        return folders.find(dirname)->second.get();
    }
    auto folder_view() { return folders | views::values; }
    size_t get_total_size() {
        if (!total_size_setted) {
            total_size_setted = true;
            total_size = accumulate(
                folder_view(), size,
                [](auto s, auto f_ptr) { return s + f_ptr->get_total_size(); });
        }
        return total_size;
    }
    auto get_name() { return name; }
};

struct FileSystem {
    Folder root{"/", nullptr};
    Folder* cwd{&root};

    void cd(Folder* const f) { cwd = f; }
    void cdup() { cd(cwd->get_parent()); }
    void cdroot() { cd(&root); }
    void cd(std::string_view dir) {
        if (dir == "/")
            cdroot();
        else if (dir == "..")
            cdup();
        else
            cwd = cwd->get_folder(dir);
    }
    void mkdir(const std::string& dir) { cwd->add_folder(dir); }
    void mkcd(const std::string& dir) {
        if (dir != "/" && dir != "..") mkdir(dir);
        cd(dir);
    }
    void touch(size_t size, const std::string& name) {
        cwd->add_file(size, name);
    }
    auto pwd() {
        std::string pwd{""};
        for (auto dir_ptr = cwd; dir_ptr; dir_ptr = dir_ptr->get_parent()) {
            pwd = dir_ptr->get_name() + pwd;
        }
        return pwd;
    }

    auto rfind(const auto& filter,
               auto ordering_lambda = std::less<Folder*>()) {
        std::set<Folder*, decltype(ordering_lambda)> rfind_result;
        rfind_internal(rfind_result, filter);
        return rfind_result;
    }

   private:
    void rfind_internal(auto& rfind_result, const auto& filter) {
        for (auto subf : cwd->folder_view()) {
            cd(subf.get());
            rfind_internal(rfind_result, filter);
            cdup();
        }
        if (filter(cwd)) rfind_result.insert(cwd);
    }
};

// 1845346
int main(int argc, char* argv[]) {
    using namespace std::literals;

    // quick and dirty
    assert(argc == 2);

    const auto input = AoC::get_input(argv[1], "\n");
    FileSystem fs;
    for (const auto& cmd_out : input) {
        const auto splitted = AoC::split(cmd_out, ' ');
        if (splitted.front() == "$") {
            if (splitted[1] == "cd") {
                fs.mkcd(splitted.back());
            }
        } else {
            if (splitted.front() == "dir") {
                fs.mkdir(splitted.back());
            } else {
                fs.touch(std::stoull(splitted.front()), splitted.back());
            }
        }
    }
    fs.cdroot();
    constexpr size_t total_space = 70000000ULL;
    constexpr size_t needs_space = 30000000ULL;
    const auto occupied_space = fs.root.get_total_size();
    const auto free_space = total_space - occupied_space;
    const auto to_be_freed = needs_space - free_space;
    auto candidate_folders = fs.rfind(
        [to_be_freed](auto f) { return f->get_total_size() >= to_be_freed; },
        [](auto rhs, auto lhs) {
            return rhs->get_total_size() < lhs->get_total_size();
        });
    std::cout << (*candidate_folders.begin())->get_total_size() << std::endl;
    return 0;
}
