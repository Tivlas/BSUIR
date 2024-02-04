#include <iostream>

#include "file.h"

void vigenereEncrypt(std::string& text, const std::string& key) {
    int keyLength = key.size();
    int textLength = text.size();

    for (int i = 0; i < textLength; ++i) {
        char& c = text[i];
        if (std::isupper(c)) {
            c = (c - 'A' + std::toupper(key[i % keyLength]) - 'A') % 26 + 'A';
        } else if (std::islower(c)) {
            c = (c - 'a' + std::toupper(key[i % keyLength]) - 'A') % 26 + 'a';
        }
    }
}

void vigenereDecrypt(std::string& text, const std::string& key) {
    int keyLength = key.size();
    int textLength = text.size();

    for (int i = 0; i < textLength; ++i) {
        char& c = text[i];
        if (std::isupper(c)) {
            c = (c - 'A' - (std::toupper(key[i % keyLength]) - 'A') + 26) % 26 + 'A';
        } else if (std::islower(c)) {
            c = (c - 'a' - (std::toupper(key[i % keyLength]) - 'A') + 26) % 26 + 'a';
        }
    }
}

int main() {
    std::cout << "Enter file name: ";
    std::filesystem::path path;
    std::cin >> path;
    path = std::filesystem::absolute(path);
    std::string text = readFile(path);

    std::cout << "Enter key: ";
    std::string key;
    std::cin >> key;

    std::cout << "Encrypt (1) or decrypt(2): ";
    int operation;
    std::cin >> operation;

    switch (operation) {
        case 1:
            vigenereEncrypt(text, key);
            break;
        case 2:
            vigenereDecrypt(text, key);
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