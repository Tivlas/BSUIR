#include <filesystem>
#include <iostream>

#include "parser.h"

int main() {
    // try {
    /* if (argc != 2) {
        std::cout << "Specify file path!\n";
        return 0;
    } */
    // std::filesystem::path path(argv[1]);
    std::filesystem::path path = "f.txt";
    parser parser(path);
    parser.parseFile();
    std::cout << parser.getTreeStr();
    /*     } catch (const std::exception& e) {
            std::cerr << e.what() << '\n';
        } */
}