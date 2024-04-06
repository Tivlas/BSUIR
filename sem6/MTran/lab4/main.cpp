#include <iostream>
#include "semantic.h"

int main() {
    std::filesystem::path path = "f.txt";
    semantic sem(path);
    return 0;
}