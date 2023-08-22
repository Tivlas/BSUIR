
#include <iostream>

using namespace std;

int main()
{
	int x;
	int y;
	cout << "Enter x, y(Be careful! If both numbers are very large or small, then an overflow may occur):\n";
	while (!(std::cin >> hex >> x >> hex >> y)) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data!\n";
		std::cout << "Reenter x, y: ";
	}

	cout << "Select operation: '+' or '-'(enter '-' or '+'): ";
	char op = '+'; // operation
	while (!(std::cin >> op) || op != '+' && op != '-') {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data!\n";
		std::cout << "Reenter '-' or '+': ";
	}
	int sum = 0;
	if (op == '+') {
		sum = x + y;
		cout << "\nsum base-10: " << sum;
		if (sum < 0) {
			sum *= -1;
			cout << "\nsum base-16: -" << hex << sum;
		}
		else {
			cout << "\nsum base-16: " << hex << sum;
		}
	}
	else {
		sum = x - y;
		cout << "\ndifference base-10: " << sum;
		if (sum < 0) {
			sum *= -1;
			cout << "\ndifference base-16: -" << hex << sum;
		}
		else {
			cout << "\ndifference base-16: " << hex << sum;
		}
	}
	cout << endl;


	return 0;
}

