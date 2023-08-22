
/*  Для заданного одномерного массива A из N элементов проверить, что существует хотя
  бы один элемент Ai, для которого выполняется условие(вариант 7).Рекурсивную
  применять каждый раз отдельно для первой трети массива и для остальной
  части(2 / 3) массива.Рекурсивные вызовы заканчивать, когда останется только один
  или два элемента. */

#include <iostream>
#include "DynamicLib.h"

int main()
{
	int n = 0;
	std::cout << "Enter n(size of array = n)\n";
	while (!(std::cin >> n) || n <= 0) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please, check the entered data!\n";
		std::cout << "Reenter n(size of array = n)\n";
	}

	int* a = new int[n];
	for (int i = 0; i < n; i++) {
		std::cout << "Enter a[" << i << "] ";
		while (!(std::cin >> a[i])) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Please, check the entered data!\n";
			std::cout << "Reenter a[" << i << "] ";
		}
	}
	std::cout << std::endl;

	if (elementExists(a, 0, n - 1)) {
		std::cout << "Such an element exists.\n";
	}
	else {
		std::cout << "No such element exists.\n";
	}

	delete[] a;
	return 0;
}
