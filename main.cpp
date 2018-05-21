#include<iostream>
#include"fs/entry.h"

int main() {
    fs::entry e("/home/midnightas/awfsitest/aa");
    e.mkdirs();

    std::vector<fs::entry> children;
    e.get_children(children);

    for (auto& i : children)
        std::cout << i << std::endl;
    std::cout << "DONE";

    return 0;
}
