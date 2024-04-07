#include <filesystem>
#include <iostream>

#include "parser.h"

int main() {
    std::filesystem::path path = "D:\\University\\BSUIR\\sem6\\MTran\\lab4\\f.txt";
    parser parser(path);
    parser.parseFile();
    std::cout << parser.getTreeStr();
}