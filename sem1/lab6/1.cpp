
#include <iostream>

int main()
{
	char str[80];
	std::cout << "Enter text: ";
	std::cin.get(str, 80);
	std::cout << std::endl;

	char* str2 = str;//так как str нельзя изменять
	char* first = str2; //  потом запомнит адрес первой цифры

	while (*str2) {
		while (!(*str2 >= '0' && *str2 <= '9') && *str2 != '\0') {
			str2++;
		}
		if (*str2 == '\0') {
			break;
		}

		first = str2; // запомнили адрес первой цифры
		while (*str2 >= '0' && *str2 <= '9') {
			str2++; // ищем адрес после последней цифры(до точки)
		}

		if (*str2 == '.') {
			char* point = str2; // запомним адрес точки
			str2++;
			while (*str2 >= '0' && *str2 <= '9') {
				str2++;
			}
			char* end = str2;
			str2++; // чтобы не изменить значение *str2 на '\0' в следующей строке
			*end = '\0';
			if (isdigit(*(point + 1))) {
				break;
			}
		}
	}

	if (*str2 == '\0') {
		std::cout << "No such substring exists!";
	}
	else {
		std::cout <<"Desired substring: " << first << std::endl;
	}
	return 0;
}

