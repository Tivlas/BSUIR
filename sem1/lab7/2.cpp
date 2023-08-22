

#include <iostream>
#include <string>
using  namespace std;
// var 7
int main()
{
	cout << "Please enter a binary number in two's complement to convert to the reverse code.\n" <<
		"If you enter incorrect data, you may get unexpected results: ";

	string num;
	bool only_0_1 = true; // форматирование ввода
	while (only_0_1) {
		getline(cin, num);
		if (num.size() == 1 || num.size() > 32) {
			cout << "The minimum number of digits is 2 and maximum is 32. Reenter the number: ";
		}
		else if (num.find_first_not_of("01") < num.size()) {
			cout << "The number should contain only 0, 1. Reenter the number: ";
		}
		else {
			only_0_1 = false;
		}
	}
	num.insert(1, 32 - num.size(), '0');// 32 бита
	cout << "\nTwo's complement number: " << num << "\n";
	for (int i = num.size() - 1; i > 0; i--) { //  отнимаем единицу
		if (num[i] == '1') {
			num[i] = '0';
			break;
		}
		else {
			num[i] = '1';
		}
	}
	cout << "Number in reverse code:  " << num;
	cout << endl;
	return 0;
}

