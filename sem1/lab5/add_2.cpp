// Рекурсивная функция из условия сводится к возведению в степень по модулю(k^n mod m)
// Реализуем алгоритм быстрого возведения в степень

#include <iostream>
unsigned long long powerMod(unsigned long long k, unsigned long long n, unsigned long long m) {
    if (n == 0) {
        return 1;
    }
    unsigned long long res = powerMod(k, n / 2, m);
    if (n % 2 == 0) {
        return (res * res) % m;
    }
    return (k * res * res) % m;
}

int main() {
    int cur = 0; // case# счетчик
    while (true) {
        cur++;
        long long k, n;
        int t;
        std::cout << "Enter k, n, t(if you wanna exit the program enter 0, 0, 0): ";
        while (!(std::cin >> k >> n >> t) || t < 1 || t > 9 || k <= 0 || n < 0 ) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Reenter k, n, t(if you wanna exit the program enter 0, 0, 0): ";
        }
        if (k == 0 && n == 0 && t == 0) {
            break;
        }
        unsigned long long m = 1; 
        for (int i = 0; i < t; i++) {
            m *= 10;  // m = 10^t, условие
        }
        k %= m; // k^n mod m == (k mod m)^n mod m
        std::cout << "Case #" << cur << ": " << powerMod(k, n, m) << std::endl << std::endl;
    }
    return 0;
}