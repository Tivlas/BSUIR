#include <iostream>

int f(int n) {
	if (n % 10 > 0) {
		return n % 10;
	}
	else if (n == 0) {
		return 0;
	}
	else {
		return f(n / 10);
	}

}

int s(int p, int q) {
	int sum = 0;
	for (int i = p; i <= q; i++) {
		sum += f(i);
	}
	return sum;
}

int main() {

	while (1) {
		int p, q;
		std::cout << "Enter p and q(if you wanna exit the program enter p = -1 and q = -1): ";
		while (!(std::cin >> p >> q) || p > q) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Renter p and q(if you wanna exit the program enter p = -1 and q = -1): ";
		}
		if (p < 0 && q < 0) {
			break;
		}
		std::cout << s(p, q) << std::endl;
	}
	return 0;
}