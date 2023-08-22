

#include <iostream>

void copy(char* str1, char* str2) {
	while (*str2) {
		*str1 = *str2;
		str1++;
		str2++;
	}
	*str1 = *str2;
}


int compare(char* str1, char* str2) {
	while (*str1 || *str2) {
		if (*str1 != *str2) {
			if (*str1 > *str2) {
				std::cout << "\nFirst string is bigger.\n";
				return 1;
			}
			else {
				std::cout << "\nSecond string is bigger.\n";
				return -1;
			}
		}
		str1++;
		str2++;
	}
	std::cout << "\nStrings are equal.\n";
	return 0;
}


void concatenate(char* str1, char* str2) {
	int start = strlen(str1);
	int end = start + strlen(str2);
	for (int i = start; i <= end; i++) {
		str1[i] = *str2;
		str2++;
	}
	std::cout << str1 << std::endl;
}


int main()
{
	char str1[200];
	char str2[200];
	std::cout << "Enter first string: ";
	std::cin.get(str1, 100);
	std::cin.ignore(1);
	std::cout << "Enter second string: ";
	std::cin.get(str2, 100);

	std::cout << std::endl<<"Concatenated strings: ";
	concatenate(str1, str2);

	compare(str1, str2);

	copy(str1, str2);
	std::cout << std::endl << "Changed first string: " << str1;
	std::cout << std::endl;




	return 0;
}

