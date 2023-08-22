/* Дан двумерный динамический массив целых чисел А размерностью n * k. Значения элементов массива
ввести с клавиатуры. Создать динамический массив из элементов, расположенных на
главной диагонали матрицы и имеющих четное значение. Вычислить произведение
элементов динамического массива. */

#include <iostream>

void createNewArray(int** a, int* b, int n, int numberOfEven, int product) {
	int number = -1; // чтобы понять какому элементу из b присвоить а[i][j]: елси нужный элемент найден, то number++
	for (int i = 0, j = 0; i < n && j < n; i++, j++) {
		if (a[i][j] % 2 == 0) {
			number++;
			b[number] = a[i][j];
		}
	}
	if (number == -1) { 
		std::wcout << " no required elements were found.\n";
	}
	else {
		for (int i = 0; i < numberOfEven; i++) {
			std::cout << b[i] << " ";
		}
		std::cout << std::endl;
		std::cout << "Product of elements: " << product << std::endl;
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

	int numberOfEven = 0; // размер нового массива
	int product = 1; // произведние элементов нового массива
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < k; j++) {
			std::cout << "Enter element a[" << i << "][" << j << "] ";
			while (!(std::cin >> a[i][j])) {
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
				std::cout << "Please, check the entered data!\n";
				std::cout << "Reenter element a[" << i << "][" << j << "] \n";
			}
			if (i == j && a[i][j] % 2 == 0) {
				numberOfEven += 1;
				product *= a[i][j];
			}
		}
	}
	int* b = new int[numberOfEven];

	std::cout << "\nNew array:";
	createNewArray(a, b, n, numberOfEven, product);
	

	for (int i = 0; i < n; i++) {
		delete[] a[i];
	}
	delete[] a;
	delete[] b;
	return 0;
}

