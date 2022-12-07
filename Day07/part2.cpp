#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <range/v3/all.hpp>
#include <set>
#include <string_view>

#include "utils.hpp"

using namespace ranges;

struct Folder {
    std::string name{};
    Folder* parent;
    std::map<std::string, size_t> files;
    std::map<std::string, std::shared_ptr<Folder>> folders;

   private:
    size_t size{0};
    bool size_setted{false};

   public:
    Folder(auto name, auto parent) : name(name), parent(parent) {}
    size_t get_size() {
        if (!size_setted) {
            size_setted = true;
            size += accumulate(files | views::values, 0ULL);
            size += accumulate(
                folders | views::values, 0ULL,
                [](auto s, const auto f_ptr) { return s + f_ptr->get_size(); });
        }
        return size;
    }
};

struct FolderTree {
    Folder root;
    FolderTree() : root("", nullptr) {}
};

void search_sizes(Folder* f, size_t at_least, std::string cwd, auto& size_set) {
    cwd += f->name;
    for (auto subf : f->folders | views::values) {
        search_sizes(subf.get(), at_least, cwd, size_set);
    }
    if (f->get_size() >= at_least) size_set.insert(f->get_size());
}

// 1845346
int main(int argc, char* argv[]) {
    using namespace std::literals;

    // quick and dirty
    assert(argc == 2);

    const auto input = AoC::get_input(argv[1], "\n");
    std::string cwd{"/"};
    FolderTree tree;
    Folder* cwf = &tree.root;
    for (const auto& cmd_out : input) {
        const auto splitted = AoC::split(cmd_out, ' ');
        if (splitted.front() == "$") {
            if (splitted[1] == "cd") {
                if (splitted.back() == "/") {
                    cwd = "/";
                    cwf = &tree.root;
                } else if (splitted.back() == "..") {
                    cwd = cwd.substr(0, cwd.find_last_of('/'));
                    cwf = cwf->parent;
                } else {
                    cwd += "/" + splitted.back();
                    cwf = cwf->folders[splitted.back()].get();
                }
            }
        } else {
            if (splitted.front() == "dir") {
                cwf->folders[splitted.back()] =
                    std::make_shared<Folder>(splitted.back(), cwf);
            } else {
                cwf->files[splitted.back()] = std::stoull(splitted.front());
            }
        }
    }

    constexpr size_t total_space = 70000000ULL;
    constexpr size_t needs_space = 30000000ULL;
    const auto occupied_space = tree.root.get_size();
    const auto free_space = total_space - occupied_space;
    const auto to_be_freed = needs_space - free_space;
    std::set<size_t> candidate_folders{};
    search_sizes(&tree.root, to_be_freed, "/", candidate_folders);

    std::cout << *candidate_folders.begin() << std::endl;
    return 0;
}
