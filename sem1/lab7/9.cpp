
#include <iostream>

using namespace std;

int get_num_3(int num_10) {
	int remainder;// остаток от деления при переводе в новую систему
	if (num_10 == 0) {
		return 0;
	}
	else {
		remainder = num_10 % 3;
		num_10 /= 3;
		get_num_3(num_10);
		if (remainder == 0) {
			cout << 3;
		}
		else {
			cout << remainder;
		}
	}
	return 0;
}

int main()
{

	int n;
	cout << "Enter number: ";
	while (!(cin >> n) || n < 1 || n>2e9) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please check the entered data!\n";
		cout << "Reenter the number: ";
	}
	cout << "\nResult: ";
	get_num_3(n);
	cout << "\n";

	return 0;
}

