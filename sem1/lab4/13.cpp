

#include <iostream>

int main()
{
    int n, am = 1; // amount
    std::cout << "Enter the number of extension cords: ";
    while (!(std::cin >> n) || n <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter the number of extension cords: ";
    }
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    for (int i = 1; i <= n; i++) {
        int cur;
        std::cout << "Enter the  number of sockets in " << i << " extension cord:\n";
        while (!(std::cin >> cur) || cur <= 0) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "\nPlease check the entered data.\n";
            std::cout << "Enter the  number of sockets in " << i << " extension cord:\n";
        }
        am += cur - 1;
    }
    std::cout << "Number of available sockets: " << am << std::endl;
	return 0;
}
