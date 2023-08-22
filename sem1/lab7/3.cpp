

#include <iostream>
#include <string>

using namespace  std;

void get_reverse_code(string& a, string& b) {
	if (a[0] == '1') {
		for (int i = 1; i < a.size(); i++) {
			if (a[i] == '0') {
				a[i] = '1';
			}
			else {
				a[i] = '0';
			}
		}
	}
	if (b[0] == '1') {
		for (int i = 1; i < b.size(); i++) {
			if (b[i] == '0') {
				b[i] = '1';
			}
			else {
				b[i] = '0';
			}
		}
	}


}

void find_sum(string& a, string& b, string& sum) {
	for (int i = sum.size() - 1; i > 0; i--) {
		sum[i] += a[i] + b[i] - '0';
		if (sum[i] > '1') {
			sum[i] -= 2;
			sum[i - 1]++;
		}
	}
	sum[0] += a[0] + b[0] - '0';
	if (sum[0] > '1') {
		sum[0] -= 2;
		for (int i = sum.size() - 1; i > 0; i--) { // если произошло переполнение старшего(знакового) разряда,
												   // то к младшему разряду +1
			if (sum[i] == '1') {
				sum[i] = '0';
			}
			else {
				sum[i] = '1';
				break;
			}
		}
	}
}

int main()
{
	string a, b;
	// проверка ввода
	bool only_0_1 = true;
	cout << "Enter 1st number: ";
	while (only_0_1) {
		getline(cin, a);
		if (a.size() == 1 || a.size() > 32) {
			cout << "The minimum number of digits is 2 and maximum is 32. Reenter the 1st number: ";
		}
		else if (a.find_first_not_of("01") < a.size()) {
			cout << "The number should contain only 0, 1. Reenter the 1st number: ";
		}
		else {
			only_0_1 = false;
		}
	}

	cout << "Enter 2nd number: ";
	only_0_1 = true;
	while (only_0_1) {

		getline(cin, b);
		if (b.size() == 1 || b.size() > 32) {
			cout << "The minimum number of digits is 2 and maximum is 32. Reenter the 2nd number: ";
		}
		else if (b.find_first_not_of("01") < b.size()) {
			cout << "The number should contain only 0, 1. Reenter the 2nd number: ";
		}
		else {
			only_0_1 = false;
		}
	}

	a.insert(1, 32 - a.size(), '0');// будет выводить 32 бита
	b.insert(1, 32 - b.size(), '0');
	cout << "\nThe 1st number: " << a << "\n";
	cout << "The 2nd number: " << b << "\n\n";
	get_reverse_code(a, b);

	string sum;
	sum.resize(32);
	for (int i = 0; i < 32; i++) {
		sum[i] = '\0';
	}

	find_sum(a, b, sum);

	if (a[0] == '0' && b[0] == '0' && sum[0] == '1' || a[0] == '1' && b[0] == '1' && sum[0] == '0') {//проверка на переполнение
		cout << "Overflow!\n";
	}
	else {
		if (sum[0] == '1') { // если знаковый бит суммы == 1, то инвертируем остальные биты
			for (int i = 1; i < sum.size(); i++) {
				if (sum[i] == '0') {
					sum[i] = '1';
				}
				else {
					sum[i] = '0';
				}
			}
		}
		cout << "sum: " << sum << endl;
	}
	return 0;
}

