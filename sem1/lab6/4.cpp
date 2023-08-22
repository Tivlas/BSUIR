
#include <iostream>
#include <string>

void replace_c(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (toupper(str[i]) == 'C' && (toupper(str[i + 1]) == 'E' || toupper(str[i + 1]) == 'I' || toupper(str[i + 1]) == 'Y')) {
			str[i] += 's' - 'c';
		}
		else if (toupper(str[i]) == 'C') {
			str[i] += 'k' - 'c';
		}
	}
}

void replace_q_qu(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (toupper(str[i]) == 'Q' && toupper(str[i + 1]) != 'U') {
			str[i] += 'k' - 'q';
		}
		else if (toupper(str[i]) == 'Q' && toupper(str[i + 1]) == 'U') {
			str[i + 1] += 'v' - 'u';
			str[i] += 'k' - 'q';
		}
	}
}

void replace_x(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (toupper(str[i]) == 'X') {
			str[i] += 'k' - 'x';
			for (int j = strlen(str) + 1; j > i + 1; j--) { //сдвигаем все элементы после 'x' вправо на одну позицию 
				str[j] = str[j - 1];
			}
			str[i + 1] = 's';
		}
	}
}

void replace_w(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (toupper(str[i]) == 'W') {
			str[i] += 'v' - 'w';

		}
	}
}

void replace_ph(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (toupper(str[i]) == 'P' && toupper(str[i + 1]) == 'H') {
			str[i] += 'f' - 'p';
			for (int j = i + 1; j < strlen(str); j++) { //сдвигаем все элементы после 'p' влево на одну позицию 
				str[j] = str[j + 1];
			}
		}
	}
}

void replace_you(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (toupper(str[i]) == 'Y' && toupper(str[i + 1]) == 'O' && toupper(str[i + 2]) == 'U') {
			str[i] += 'u' - 'y';
			for (int j = i + 1; j <= strlen(str) - 2; j++) { //сдвигаем все элементы после 'u' влево на две позиции 
				str[j] = str[j + 2];
			}

		}
	}
}

void replace_oo(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (toupper(str[i]) == 'O' && toupper(str[i + 1]) == 'O') {
			str[i] += 'u' - 'o';
			for (int j = i + 1; j < strlen(str); j++) { //сдвигаем все элементы после 'o' влево на одну позицию 
				str[j] = str[j + 1];
			}
		}
	}
}

void replace_ee(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (toupper(str[i]) == 'E' && toupper(str[i + 1]) == 'E') {
			str[i] += 'i' - 'e';
			for (int j = i + 1; j < strlen(str); j++) { //сдвигаем все элементы после 'o' влево на одну позицию 
				str[j] = str[j + 1];
			}
		}
	}
}

void replace_th(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (toupper(str[i]) == 'T' && toupper(str[i + 1]) == 'H') {
			str[i] += 'z' - 't';
			for (int j = i + 1; j < strlen(str); j++) { //сдвигаем все элементы после 'h' влево на одну позицию 
				str[j] = str[j + 1];
			}
		}
	}
}

void replace_double_consonants(char* str) {
	for (int i = 0; i < strlen(str); i++) {
		if (isalpha(str[i])) {
			if (toupper(str[i]) == toupper(str[i + 1]) && toupper(str[i]) != 'A' && toupper(str[i]) != 'E' && toupper(str[i]) != 'I' &&
				toupper(str[i]) != 'O' && toupper(str[i]) != 'U' && toupper(str[i]) != 'Y') {
				for (int j = i + 1; j < strlen(str); j++) { //сдвигаем все элементы после повторного влево на одну позицию 
					str[j] = str[j + 1];
				}
			}
		}
	}
}


int main()
{
	char str[200];
	std::cout << "Enter string: ";
	std::cin.get(str, 100);

	replace_c(str);
	replace_q_qu(str);
	replace_x(str);
	replace_w(str);
    replace_ph(str);
	replace_you(str);
	replace_oo(str);
	replace_ee(str);
	replace_th(str);
	replace_double_consonants(str);

	std::cout << std::endl;
	std::cout << str;
	std::cout << std::endl;
	return 0;

	// Too swift for Theex, too quick for Gallow, Too strong for young Prince Joseph to follow.
	
}

