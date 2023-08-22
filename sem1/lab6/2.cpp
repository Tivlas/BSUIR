
#include <iostream>
#include <string>

int main()
{
	char str[200];
	std::cout << "Enter text: ";
	std::cin.get(str, 200, '\n');

	char* str2 = str;
	while (*str2) {
		if (*str2>='A' && *str2<='Z' || *str2>='a' && *str2<='z') {
			std::cout << *str2;
		}
		str2++;
	}

	return 0;
}

