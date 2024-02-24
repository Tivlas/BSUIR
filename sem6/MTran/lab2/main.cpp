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
    if (argc != 2) {
        std::cout << "Specify file path!\n";
        return 0;
    }
    std::filesystem::path path(argv[1]);
    path = std::filesystem::absolute(path);
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cout << "Invalid file! " + path.string();
        return 0;
    }

    std::string code;
    std::string line;

    while (std::getline(file, line)) {
        code += line + '\n';
    }

    file.close();
    lexer lexer(code);

    lexer.tokenize();

    return 0;
}