#include <iostream>
#include <filesystem>
#include "parser.h"

int main() {
    /* if (argc != 2) {
        std::cout << "Specify file path!\n";
        return 0;
    } */
    // std::filesystem::path path(argv[1]);
    std::filesystem::path path = "f.txt";
    parser parser(path);
    parser.parseFile();
    std::cout << "Done...\n";
}