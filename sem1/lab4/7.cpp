

#include <iostream>


int main()
{
    int n = 1;
    std::cout << "Enter number(n): \n";
	while (!(std::cin >> n) || n <= 0) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nPlease check the entered data.\n";
		std::cout << "Enter number(n):\n";
		std::cout << std::endl;
	}
	
	int k = n;
	int amount = 0;
	while (n) {
		n /= 10;
		amount++;
	}
	std::cout << "Amount of digits = " << amount << std::endl;
	
	int cur = 0;
	int i = 0;
	std::cout << "Enter number of digit(i):\n";
	while (!(std::cin >> i) || i > amount-1 || i >= 10) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data.\n";
		std::cout << "Reenter number of digit(i):\n";
	}
	n = k;
	while (n) {
		if (i == cur) {
			std::cout << "Required digit is  " << n % 10 << std::endl;
		}
		n /= 10;
		cur++;
	}
	
    return 0;
}

