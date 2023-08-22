
#include <iostream>

long long oddDivisor(long long n) {
    long long k = (n + 1) / 2; // число нечетных до n
    if (0 == n) {
        return 0;
    }
    return k * k + oddDivisor(n / 2);
 }

int main()
{
    while (true) {
       long long n;
        std::cout << "Enter n(if you wanna exit the program enter 0): ";
        while (!(std::cin >> n) ||  n > 1000000000) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Reenter n(if you wanna exit the program enter 0): ";
        }
        if (n == 0) {
            break;
        }
        std::cout << "Result: " << oddDivisor(n) << std::endl << std::endl;
    }
    return 0;
}

