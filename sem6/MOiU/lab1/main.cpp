#include <iostream>
#include <vector>

#include "algorithm.h"

int main() {
    size_t size;
    std::cout << "Enter n: ";
    std::cin >> size;

    Algorithm::Matrix A(size, std::vector<double>(size, 0)),
        A_inverse(size, std::vector<double>(size, 0));
    std::cout << "Enter matrix A: ";
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            std::cin >> A[i][j];
        }
    }
    std::cout << "Enter matrix A_inverse: ";
    for (size_t i = 0; i < size; i++) {
        for (size_t j = 0; j < size; j++) {
            std::cin >> A_inverse[i][j];
        }
    }

    std::vector<double> x(size);
    std::cout << "Enter vector x: ";
    for (auto& elem : x) {
        std::cin >> elem;
    }

    size_t I;
    std::cout << "Enter i: ";
    std::cin >> I;

    Algorithm alg(A, A_inverse, I, x);
    auto ans = alg.get_inverse();
    if (ans.empty())
        std::cout << "No answer!";
    else {
        for (size_t i = 0; i < size; i++) {
            for (size_t j = 0; j < size; j++) {
                std::cout << ans[i][j] << ' ';
            }
            std::cout << '\n';
        }
    }
    return 0;
}

/*
3
1 -1 0 0 1 0 0 0 1
1 1 0 0 1 0 0 0 1
1 0 1
2


4
1 2 3 4 2 3 1 2 1 1 1 -1 1 0 -2 -6
22 -6 -26 17 -17 5 20 -13 -1 0 2 -1 4 -1 -5 3
0 -1 -2 -3 
2

*/