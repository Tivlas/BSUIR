#include <math.h>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <stack>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include "lexer.h"
#include "token.h"

void SetConsoleRedColor() { std::cout << "\033[31m"; }

void SetConsoleYellowColor() { std::cout << "\033[33m"; }

void ResetConsoleColor() { std::cout << "\033[0m"; }

int main(int argc, char *argv[]) {
    /* if (argc != 2) {
        std::cout << "Specify file path!\n";
        return 0;
    } */
    //std::filesystem::path path(argv[1]);
    std::filesystem::path path = "test-files/f1.txt";
    lexer lexer(path);
    lexer.tokenize();
    lexer.print_all();
    return 0;
}