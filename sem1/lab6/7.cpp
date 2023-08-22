
#include <iostream>
#include <string>

int main()
{
	std::string s;
	bool onlyLetters = true; // lower case letters
	std::cout << "Enter a word: ";
	while (onlyLetters) {
		getline(std::cin, s);
		if (s.size() > 105 || s.size() == 0) {
			std::cout << "The maximum number of letters is 105 and minimum is 1. Reenter the word: ";
		}
		else if (s.find_first_not_of("abcdefghijklmnopqrstuvwxyz") <= s.size()) {
			std::cout << "The word should contain only lower case letters. Reenter the word: ";
		}
		else {
			onlyLetters = false;
		}
	}

	bool* exists = new bool[s.size()+1]; // элемент exist[i] будет показывать, существует ли палиндром длины i
	for (int i = 0; i < (int)s.size()+1; i++) {
		exists[i] = false;
	}

	int odd = 1;// для определения длины палиндрома нечётной длины
	int even = 0;// для определения длины палиндрома чётной длины
	for (int i = 0; i < (int)s.size(); i++) {

		odd = 1;
		while (i - odd >= 0 && i + odd <= (int)s.size() && s[i - odd] == s[i + odd]) {
			odd++;
		}
		int k = 1; // нечетное число = 2k-1
		while (k <= odd ) {
			exists[k * 2 - 1] = true;
			k++;
		}
		// even = 0
		while (i - even - 1 >= 0 && i + even <= (int)s.size() && s[i - even - 1] == s[i + even]) {
			even++;
		}
		k = 0;
		while (k <= even ) {
			exists[k * 2] = true;
			k++;
		}
	}

	int res = -1;
	for (int i = 0; i < (int)s.size()+1; i++) {
		std::cout << exists[i] << " ";
	}
	for (int i = (int)s.size(); i > 0; i--) {
		if (!exists[i]) {
			res = i;
			break;
		}
	}
	std::cout << "\nResult: " << res << std::endl;
	delete[] exists;
    return 0;
}

