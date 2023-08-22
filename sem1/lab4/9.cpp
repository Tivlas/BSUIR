
#include <iostream>

int main()
{
	std::cout << "The program will multiply the square matrices you entered(C=A*B).\n" << std::endl;
	int n;
	std::cout << "Enter n(size of matrix = n*n)\n";
	while (!(std::cin >> n) || n <= 0) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data.\n";
		std::cout << "Enter n(size of matrix = n*n)\n";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	
	int** a = new int* [n];
	for (int i = 0; i < n; i++) {
		a[i] = new int[n];
	}

	int** b = new int* [n];
	for (int i = 0; i < n; i++) {
		b[i] = new int[n];
	}
	
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << "Enter a[" << i << "][" << j << "]" << std::endl;
			while (!(std::cin >> a[i][j]) || a[i][j] < 0 || a[i][j] > 1e6) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\nPlease check the entered data.\n";
				std::cout << "Enter a[" << i << "][" << j << "]" << std::endl;
			}
		}
	}
	std::cout << std::endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << "Enter b[" << i << "][" << j << "]" << std::endl;
			while (!(std::cin >> b[i][j]) || b[i][j] < 0 || b[i][j] > 1e6) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "\nPlease check the entered data.\n";
				std::cout << "Enter b[" << i << "][" << j << "]" << std::endl;
			}
		}
	}
	std::cout << std::endl;
	std::cout << "Matrix A:\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << a[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Matrix B:\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << b[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	
	int** c = new int* [n];
	for (int i = 0; i < n; i++) {
		c[i] = new int[n];
	}
	std::cout << "Matrix C=A*B:\n";
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			c[i][j] = 0;
			for (int k = 0; k < n; k++)
				c[i][j] += a[i][k] * b[k][j];
			std::cout << c[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	
	for (int i = 0; i < n; i++) {
		delete[] a[i];
	}
	delete[] a;
	for (int i = 0; i < n; i++) {
		delete[] b[i];
	}
	delete[] b;
	for (int i = 0; i < n; i++) {
		delete[] c[i];
	}
	delete[] c;
}

