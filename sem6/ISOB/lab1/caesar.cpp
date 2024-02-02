#include <iostream>

#include "file.h"

void caesarEncrypt(std::string& text, int shift) {
    for (char& c : text) {
        if (std::isupper(c)) {
            c = (c - 'A' + shift) % 26 + 'A';
        } else if (std::islower(c)) {
            c = (c - 'a' + shift) % 26 + 'a';
        }
    }
}

void caesarDecrypt(std::string& text, int shift) {
    shift %= 26;
    for (char& c : text) {
        if (std::isupper(c)) {
            c = (c - 'A' - shift + 26) % 26 + 'A';
        } else if (std::islower(c)) {
            c = (c - 'a' - shift + 26) % 26 + 'a';
        }
    }
}

int main() {
    std::cout << "Enter file name: ";
    std::filesystem::path path;
    std::cin >> path;
    path = std::filesystem::absolute(path);
    std::string text = readFile(path);

    std::cout << "Enter shift: ";
    int shift;
    std::cin >> shift;
    if (shift < 0) {
        std::cerr << "Invalid input!";
        return 0;
    }

    std::cout << "Encrypt (1) or decrypt(2): ";
    int operation;
    std::cin >> operation;

    switch (operation) {
        case 1:
            caesarEncrypt(text, shift);
            break;
        case 2:
            caesarDecrypt(text, shift);
            break;
        default:
            std::cerr << "Invalid input!";
            return 0;
    }

    std::cout << "Enter output file name: ";
    std::filesystem::path outPath;
    std::cin >> outPath;
    outPath = std::filesystem::absolute(outPath);
    writeToFile(outPath, text);
}