
#include <algorithm>
#include <iostream>

int main()
{
	int n, m;
	std::cout << "Enter n, m(size of array = n * m)\n";
	while (!(std::cin >> n >> m) || n <= 0 || m <= 0) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "\nPlease check the entered data.\n";
		std::cout << "Enter n, m(size of array = n * m)\n";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	
	int** a = new  int* [n];
	for (int i = 0; i < n; i++) {
		a[i] = new  int[m];
	}

	int** b = new  int* [n+2];
	for (int i = 0; i < n+2; i++) {
		b[i] = new  int[m+2];
	}
	
	for (int i = 0; i < n + 2; i = n + 1 + i) {
		for (int j = 0; j < m + 2; j++) {
			b[i][j] = -1;
		}
	}
	for (int j = 0; j < m + 2; j = j + m + 1) {
		for (int i = 0; i < n + 2; i++) {
			b[i][j] = -1;
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			std::cout << "Enter a[" << i << "][" << j << "]" << std::endl;
			while (!(std::cin >> a[i][j]) || a[i][j] < 0 || a[i][j] > 1e6) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\nPlease check the entered data.\n";
				std::cout << "Enter a[" << i << "][" << j << "]" << std::endl;
			}
			b[i+1][j+1] = std::max({ a[i][j], b[i][j+1], b[i+1][j] });
		}
	}
	std::cout << std::endl;
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j <  m; j++) {
			std::cout << a[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			std::cout << b[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	
	for (int i = 0; i < n; i++) {
		delete[] a[i];
	}
	delete[] a;
	for (int i = 0; i < n+2; i++) {
		delete[] b[i];
	}
	delete[] b;
	return 0;
}

