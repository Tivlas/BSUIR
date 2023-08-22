
#include <iostream>
#include <string>
#include <cmath>

using namespace std;
//var 7
int get_num_13(int num_10) {
	int remainder;// остаток от деления при переводе в 13ную
	if (num_10 == 0) {
		return 0;
	}
	else {
		remainder = num_10 % 13;
		num_10 /= 13;
		get_num_13(num_10);
		if (remainder < 10) {
			cout << remainder;
		}
		else {
			cout << (char)('a' + remainder % 10);
		}
	}
	return 0;
}

// 6 -> 13
int main()
{
	cout << "Enter a base-6 number: ";
	string number;
	bool onlyDigits = true;
	while (onlyDigits) {
		getline(cin, number);
		if (number.size() == 0 || number.size() > 10) {
			cout << "The minimum number of digits is 1 and maximum is 10. Reenter the number: ";
		}
		else if (number.find_first_not_of("012345") <= number.size()) {
			cout << "The number should contain only 0, 1, 2, 3, 4, or 5. Reenter the number: ";
		}
		else {
			onlyDigits = false;
		}
	}

	// without array
	int num_6 = stoi(number);// число в шестеричной
	int num_10 = 0;// это же число в десятичной
	int power = 0;// степень 6ки для переволда в десятичную
	while (num_6) {
		num_10 += (num_6 % 10) * pow(6, power);
		num_6 /= 10;
		power++;
	}
	cout << "Base-10: " << num_10 << endl;
	cout << "Base-13: ";
	get_num_13(num_10);
	cout << endl;

	//using array
	int a[1];
	a[0] = stoi(number);
	num_10 = 0;// это же число в десятичной
	power = 0;// степень 6ки для переволда в десятичную
	while (a[0]) {
		num_10 += (a[0] % 10) * pow(6, power);
		a[0] /= 10;
		power++;
	}
	cout << "\nUsing an array: " << endl;
	cout << "Base-10: " << num_10 << endl;
	cout << "Base-13: ";
	get_num_13(num_10);
	return 0;
}

