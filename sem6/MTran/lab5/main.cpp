#include <iostream>
#include "executer.h"

int main() {
    std::filesystem::path path = "golang/f.go";
    auto exe = std::make_shared<executer>(path);
    exe->execute();
    return 0;
}