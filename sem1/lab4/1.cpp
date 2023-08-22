

#include <iostream>

// var_7

int main(){
	const int k = 20;
	int a[k];

	for (int i = 0; i < k; i++) {
		std::cout << "Enter " << i << "-element(a[i]) of array\n";
		while (!(std::cin >> a[i])) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "\nPlease check the entered data.\n";
			std::cout << "Enter " << i << "-element(a[i]) of array\n";
		}
	}

	for (int i = 0; i < k; i++) {
		std::cout << a[i] << "  ";
	}
	std::cout << std::endl;

	std::cout << "How many positions to the right do you want to shift the elements of the array?" << std::endl;
	std::cout << "Enter number." << std::endl;
	int n = 0;
	while (!(std::cin >> n) || n <= 0 || n >= k) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nPlease check the entered data.\n";
		std::cout << "Enter number." << std::endl;
		std::cout << std::endl;
	}

	for (int j = 0; j < n; j++) {
		int cur = a[k-1];
		for (int i = k-1; i > 0; i--) {
			a[i] = a[i - 1];
		}
		a[0] = cur;
	}

	for (int i = 0; i < k; i++) {
		std::cout << a[i] << "  ";
	}
	std::cout << std::endl;

	return 0;
}
