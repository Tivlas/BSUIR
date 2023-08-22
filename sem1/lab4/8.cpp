

#include <iostream>

int main()
{
	srand(time(NULL));
	int n;
	std::cout << "Enter n(size of array = n)\n";
	while (!(std::cin >> n) || n <= 0) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data.\n";
		std::cout << "Enter N(size of array = NxN)\n";
		std::cout << std::endl;
	}
	std::cout << std::endl;
	
	int* a = new int[n];
	bool already;
	for (int i = 0; i < n;) {
		already = false;
		int value = (rand() % 1000);

		for (int cur = 0; cur < i; cur++) {
			if (a[cur % n] == value) {
				already = true;
				break;
			}
		}

		if (!already) {
			a[i] = value;
			i++;
		}
	}

	for (int i = 0; i < n; i++) {
		std::cout << a[i] << " ";
	}
	std::cout << std::endl;
	//std::cout << std::endl;

	int cmp1 = -1;
	int cmp2 = INT_MAX;
	int min = 0;
	int max = 0;
	int i2 = 0; // min
	int i1 = 0; // max

	// поиск МИНИМАЛЬНОГО
	for (int i = 0; i < n; i++) {
		if (a[i] < cmp2) {
			cmp2 = a[i];
			min = a[i];
			i2 = i;
		}
	}

	// поиск МАКСИМАЛЬНОГО
	for (int i = 0; i < n; i++) {
		if (a[i] > cmp1) {
			cmp1 = a[i];
			max = a[i];
			i1 = i;
		}
	}
	std::cout << "Minimum element = " << min << "\tindex: " << i2 << std::endl;
	std::cout << "Maximum element = " << max << "\tindex: " << i1 << std::endl;
	std::cout << std::endl;
	
	if (i1 > i2) {
		std::swap(i1, i2);
	}
	int am = 0; // amount of 0	
	int k = n;
	for (int i = i1 + 1; i < i2; i++) {
		a[i] = 0;
		am++;
		k--;
	}
	for (int i = 0; i < n; i++) {
		std::cout << a[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "Amount of 0:  " << am << std::endl;
	std::cout << std::endl;
	
	int nonzero = n - am;
	if (am <= nonzero) {
		std::cout << "Most of the array does not contain zeros.\n";
	}
	else {
		for (int i = 0; i < n; i++) {
			if (i >= i2) {
				a[i - am] = a[i];
			}
		}

		// RESULT
		for (int i = 0; i < k; i++) {
			std::cout << a[i] << " ";
		}
		std::cout << std::endl;
	}

	delete[] a;
	return 0;
}