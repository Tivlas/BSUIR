
#include <iostream>

int main()
{
	char str[80];
	std::cout << "Enter text: ";
	std::cin.get(str, 80);
	int start = 0; //  потом  будет индексом адрес первой цифры
	int end = 0;  // потом будет границей вывода

	int i = 0;
	while (str[i] != '\0') {

		while (!(str[i] >= '0' && str[i] <= '9') && str[i] != '\0') {
			i++;
		}
		if (str[i] == '\0') {
			break;
		}

		start = i; // запомнили индекс первой цифры
		while (str[i] >= '0' && str[i] <= '9') {
			i++; // ищем индекс точки(не цифры)
		}

		if (str[i] == '.') {
			int point = i; // запомним индекс точки
			i++;
			while (str[i] >= '0' && str[i] <= '9') {
				i++;
			}
			end = i;
			i++; // чтобы не изменить значение str[i] на '\0' в следующей строке
			str[i] = 't'; // любой символ, кроме '\0'
			str[end] = '\0';
			if (isdigit(str[point + 1])) {
				break;
			}
		}
	}

	if (str[i] == '\0') {
		std::cout << "No such substring exists!\n";
	}
	else {
		std::cout << "Desired substring: ";
		for (int i = start; i < end; i++) {
			std::cout << str[i];
		}
	}
	std::cout << std::endl;
	return 0;
}