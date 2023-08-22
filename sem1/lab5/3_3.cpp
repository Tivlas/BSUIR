/* Дан двумерный динамический массив целых чисел. Значения элементов данного 
массива ввести с клавиатуры. Создать динамический массив из элементов, 
расположенных в четных столбцах данного массива и имеющих нечетное значение. 
Вычислить среднее арифметическое элементов динамического массива.*/

#include <iostream>

void createNewArray(int** a, int n, int k, int* b, int numberOfOdd, double avrg) {
	int number = -1; // чтобы понять какому элементу из b присвоить а[i][j]: елси нужный элемент найден, то number++
	for (int i = 0; i < n; i ++ ) {
		for (int j = 0; j < k; j += 2) {
			if (a[i][j] % 2 == 1) {
				number++;
				b[number] = a[i][j];
			}
		}
	}

	if (number == -1) {
		std::wcout << " no required elements were found.\n";
	}
	else {
		for (int i = 0; i < numberOfOdd; i++) {
			std::cout << b[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Average = " << avrg << std::endl;;
	}
}

int main()
{
	int n = 0;
	int k = 0;
	std::cout << "Enter n, k(size of array = n*k)\n";
	while (!(std::cin >> n >> k) || k <= 1 || n <= 1) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please, check the entered data!\n";
		std::cout << "Reenter n, k(size of array = n*k)\n";
	}

	int** a = new int* [n];
	for (int i = 0; i < n; i++) {
		a[i] = new int[k];
	}

	int numberOfOdd = 0; // size of new array
	int sum = 0; // sum of odd elements

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			std::cout << "Enter element a[" << i << "][" << j << "] ";
			while (!(std::cin >> a[i][j])) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Please, check the entered data!\n";
				std::cout << "Reenter element a[" << i << "][" << j << "] \n";
			}
			if (j % 2 == 0 && a[i][j] % 2 == 1) {
				numberOfOdd++;
				sum += a[i][j];
			}
		}
	}
	std::cout << std::endl;
	double avrg = sum / numberOfOdd;

	int* b = new int[numberOfOdd];
	createNewArray(a, n, k, b, numberOfOdd,avrg);
	std::cout << std::endl;

	for (int i = 0; i < n; i++) {
		delete[] a[i];
	}
	delete[] a;
	delete[] b;
	return 0;
}

