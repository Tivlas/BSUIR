
#include <algorithm>
#include <iostream>

int main()
{
	std::cout << "Enter n(size of array = n^3)\n";
	int n;
	while(!(std::cin >> n) || n <= 1) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data.\n";
		std::cout << "Enter n(size of array = n^3)\n";
	}
	std::cout << std::endl;
	int*** a = new int** [n];
	for (int i = 0; i < n; i++) {
		a[i] = new int* [n];
		for (int j = 0; j < n; j++) {
			a[i][j] = new int[n];
		}
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			for (int k = 0; k < n; k++) {
				std::cout << "Enter a[" << i << "][" << j << "][" << k << "]" << std::endl;
				while (!(std::cin >> a[i][j][k]) || a[i][j][k] < 0) {
					std::cin.clear();
					std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
					std::cout << "Please check the entered data.\n";
					std::cout << "Enter a[" << i << "][" << j << "][" << k << "]" << std::endl;
				}
			}
		}
	}
	
	int sum1 = 0;
	int sum2 = 0;
	int sum3 = 0;
	int sum4 = 0;
	for (int i = 0; i < n; i++) {
		sum1 += a[i][i][i]; // from a[0][0][0] to a[n-1][n-1][n-1]
		sum2 += a[n - i -1][i][i]; // from a[n-1][0][0] to a[0][n-1][n-1]
		sum3 += a[i][n - i - 1][i]; // from a[0][n-1][0] to a[n-1][0][n-1]
		sum4 += a[n - i - 1][n - i - 1][i]; // from a[n-1][n-1][0] to a[0][0][n-1]
	}
	std::cout << std::endl;
	std::cout << "sum1 (from a[0][0][0] to a[" << n - 1 << "][" << n - 1 << "][" << n - 1 << "]) = " << sum1 << std::endl;
	std::cout << "sum2 (from a[" << n - 1 << "][0][0] to a[0][" << n - 1 << "][" << n - 1 << "]) = " << sum2 << std::endl;
	std::cout << "sum3 (from a[0][" << n - 1 << "][0] to a[" << n - 1 << "][0][" << n - 1 << "]) = " << sum3 << std::endl;
	std::cout << "sum4 (from a[" << n - 1 << "][" << n - 1 << "][0] to a[0][0][" << n - 1 << "]) = " << sum4 << std::endl;
	int sum = std::max({ sum1, sum2, sum3, sum4 });
	std::cout << std::endl;
	
	if (sum1 == sum2 && sum2 == sum3 && sum3 == sum4) {
		std::cout << "The sum of the numbers is the same on all diagonals: " << sum1 << std::endl;
	} 
	else if (sum == sum1) {
		std::cout << "The largest sum of numbers on the diagonal from a[0][0][0] to a[" << n - 1 << "][" << n - 1 << "][" << n - 1 << "]: " << sum1 << std::endl;
	}
	else if (sum == sum2) {
		std::cout << "The largest sum of numbers on the diagonal from a[" << n - 1 << "][0][0] to a[0][" << n - 1 << "][" << n - 1 << "]: " << sum2 << std::endl;
	}
	else if (sum == sum3) {
		std::cout << "The largest sum of numbers on the diagonal from a[0][" << n - 1 << "][0] to a[" << n - 1 << "][0][" << n - 1 << "]: " << sum3 << std::endl;
	}
	else {
		std::cout << "The largest sum of numbers on the diagonal from a[" << n - 1 << "][" << n - 1 << "][0] to a[0][0][" << n - 1 << "]: " << sum4 << std::endl;
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; i++) {
			delete[] a[i][j];
		}
		delete[] a[i];
	}
	delete[] a;
	return 0;
}