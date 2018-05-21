#include<iostream>
#include"fs/entry.h"

/// This file does not test all functionalities of the library.
int main() {
    fs::entry e("/home/midnightas/awfsitest/aa");
    e.mkdir();
    e.mkdirs();

    std::vector<fs::entry> children;
    e.get_children(children);

    for (auto& i : children)
        std::cout << i << std::endl;

    e.remove();

    return 0;
}
