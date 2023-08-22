
#include <iostream>
#include <string>

long long fact(int n) {
	if (n < 2) {
		return 1;
	}
	return n * fact(n - 1);
}

int main()
{
	std::string s;
	bool onlyLetters = true;
	std::cout << "Enter a word: ";
	while (onlyLetters) {
		getline(std::cin, s);
		if (s.size() > 14 || s.size() == 0) {
			std::cout << "The maximum number of letters is 14 and minimum is 1. Reenter the word: ";
		}
		else if (s.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ") <= 14) {
			std::cout << "The word should contain only letters. Reenter the word: ";
		}
		else {
			onlyLetters = false;
		}
	}
	
	int size = s.size();
	int* a = new int[size] {}; // элементы массива отражают количество каждой буквы слова
	int number = 0;// количество текущей буквы
	int cur = -1; // индекс элемента массива  a[]

	for (int i = 0; i < (int)s.size();) {
		number = 0;
		char temp = s[i];
		for (int j = 0; j < (int)s.size();) {
			if (toupper(s[j]) == toupper(temp)) {
				number++;
				s.erase(j, 1);
			}
			else {
				j++;
			}
		}
		cur++;
		a[cur] = number;
	}

	long long factorial = fact(size);
	long long product = fact(a[0]);
	for (int i = 1; i < size; i++) {
		if (a[i] == 0) {
			break;
		}
		product *= fact(a[i]);
	}

	std::cout << "Result: " << factorial / product << std::endl;
	delete[] a;
	return 0;
}

