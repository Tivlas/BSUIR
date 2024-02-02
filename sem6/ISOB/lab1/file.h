#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>

std::string readFile(const std::filesystem::path& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::invalid_argument("Invalid file! " + filename.string());
    }
    std::string fileContents((std::istreambuf_iterator<char>(file)),
                             std::istreambuf_iterator<char>());
    file.close();
    return fileContents;
}

void writeToFile(const std::filesystem::path& filename,
                 const std::string& text) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::invalid_argument("Invalid file! " + filename.string());
    }
    file << text;
    file.close();
}