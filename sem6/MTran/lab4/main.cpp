#include <iostream>
#include "semantic.h"

int main() {
    std::filesystem::path path = "f.txt";
    auto sem = std::make_shared<semantic>(path);
    sem->analyze();
    return 0;
}