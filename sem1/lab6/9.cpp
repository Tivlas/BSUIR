
#include <iostream>
#include <string>

int main()
{
    std::string s;
    std::cout << "Enter string: ";
    getline(std::cin, s);
    unsigned long long sum = 0;
    unsigned long long term = 0; // текущее слагаемое
    for (int i = 0; i < static_cast<int>(s.size()); i++) {
        term = 0;
        while (isdigit(s[i]) && std::to_string(term).size() < 19 && i < (int)(s.size())) {
            term *= 10;
            term += s[i] - '0';
            i++;
        }
        sum += term;
    }
    std::cout << sum << std::endl;
    return 0;
}