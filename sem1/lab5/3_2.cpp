
/* Создать двумерный динамический массив вещественных чисел. Определить,
встречаются ли среди элеменов элементы с нулевым значением. Если встречаются, то определить их индексы и
общее количество. Переставить элементы этого массива в обратном порядке и вывести на экран.  */

#include <iostream>

void createNewArray(double** a, int n) {

	for (int i = 0; i <= ((n * n - 1) / 2) / n; i++) { // до той строки, в которой нахоится центральный элеемент
		for (int j = 0; j < n; j++) {

			int N = i * n + j; // порядковый номер элемента a[i][j]
			std::swap(a[N / n][N % n], a[((n - 1) * n + n - 1 - N) / n][((n - 1) * n + n - 1 - N) % n]);
			// меняем местами текущий элемент(с номером N) и элемент с номером = (номер последнего элемента) - N
		}
	}
	// т.к. елси n нечетное, то центральный элемент находиться ровно по центру(а не в конце строки, как для четного n),
	// то соседи(справа и слева) центрального элемента поменяются местами два раза.
	// Поменяем их местами третий раз
	if (n % 2 == 1) {
		std::swap(a[((n * n - 1) / 2 - 1) / n][((n * n - 1) / 2 - 1) % n], a[((n * n - 1) / 2 + 1) / n][((n * n - 1) / 2 + 1) % n]);
	}
}

int main()
{
	int n = 0;
	std::cout << "Enter n(size of array = n*n)\n";
	while (!(std::cin >> n) || n <= 1) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please, check the entered data!\n";
		std::cout << "Reenter n(size of array = n*n)\n";
	}
	double** a = new double* [n];
	for (int i = 0; i < n; i++) {
		a[i] = new double[n];
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << "Enter element a[" << i << "][" << j << "] ";
			while (!(std::cin >> a[i][j])) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Please, check the entered data!\n";
				std::cout << "Reenter element a[" << i << "][" << j << "] \n";
			}
		}
	}
	std::cout << std::endl;

	int numberOfZeros = 0;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			if (a[i][j] == 0) {
				numberOfZeros++;
				std::cout << "Element a[" << i << "][" << j << "] = 0" << std::endl;
			}
		}
	}
	std::cout << std::endl;

	createNewArray(a, n);
	std::cout << "Transformed array: " << std::endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			std::cout << a[i][j] << "\t";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << "Number of zeros: " << numberOfZeros << std::endl;

	for (int i = 0; i < n; i++) {
		delete[] a[i];
	}
	delete[] a;
	return 0;
}

