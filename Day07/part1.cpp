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
            size += accumulate(
                folders | views::values,
                accumulate(files | views::values, 0ULL),
                [](auto s, const auto f_ptr) { return s + f_ptr->get_size(); });
        }
        return size;
    }
};

struct FolderTree {
    Folder root;
    FolderTree() : root("", nullptr) {}
};

void search_sizes(Folder* f, std::string cwd, auto& top_size_map) {
    cwd += f->name;
    for (auto subf : f->folders | views::values) {
        search_sizes(subf.get(), cwd, top_size_map);
    }
    if (f->get_size() <= 100000) top_size_map[cwd] = f->get_size();
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
    std::map<std::string, size_t> top_folders{};
    search_sizes(&tree.root, "/", top_folders);

    std::cout << accumulate(top_folders | views::values, 0ULL) << std::endl;
    return 0;
}
