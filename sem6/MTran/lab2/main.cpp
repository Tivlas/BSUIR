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

int main(int argc, char *argv[]) {
    /* if (argc != 2) {
        std::cout << "Specify file path!\n";
        return 0;
    } */
    // std::filesystem::path path(argv[1]);
    std::filesystem::path path = "test-files/numbers.txt";
    lexer lexer(path);
    lexer.tokenize();
    lexer.print_as_table();
    lexer.print_as_token_sequence();
    return 0;
}