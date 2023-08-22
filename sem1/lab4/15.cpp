

#include <iostream>

int main()
{
	int n;
	std::cout << "Enter the number you wanna find: ";
	while (!(std::cin >> n) || n <= 0) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data.\n";
		std::cout << "Enter the number you wanna find: ";
	}
	int num = n;

	for (int i = n; ceil(sqrt(i)) != floor(sqrt(i)); i++) {
		n = sqrt(i + 1);
	}

	int** table = new  int* [n];
	for (int i = 0; i < n; i++) {
		table[i] = new  int[n];
		/*for (int j = 0; j < n; j++) {
			table[i][j] = 0;
		}*/
	}

	int cur = 1;
	int x = 0;// переменная для изменения j
	int y = 1;// переменная для изменения i
	int i = 0, j = 0;
	while (cur < num) {
		table[i][j] = cur;

		if (j == 0) {
			j = i + 1;
			i = 0;
			y = 1;
			x = 0;
			cur++;
			continue;
		}
		if (i == j) {
			y = 0;
			x = -1;
		}
		i += y;
		j += x;
		cur++;
	}
	
	/*for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << table[i][j] << "\t";
		}
		std::cout << std::endl;
	}*/
	std::cout << std::endl;
	std::cout << "Coordinates of the entered number:  " << i+1 << ";" << j+1 << std::endl;

	for (int i = 0; i < n; i++) {
		delete[] table[i];
	}
	delete[] table;
	return 0;
}
