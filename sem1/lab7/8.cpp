
#include <iostream>
#include <bitset>
#include <string>

using namespace std;

//время выполнения терпимо при n<=35 :)

int main()
{
	int n = 0;
	cout << "Enter n: ";
	while (!(cin >> n) || n < 1 || n>10000) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Please check the entered data!\n";
		cout << "Reenter n: ";
	}

	string bin_i; // i - это натуральное число, bin_i - его двоичная форма
	string s_i;// i как строка
	int remainder;// остаток от деления при переводе в двоичную;
	unsigned long long temp = 0;// хранить значение i

	bool found = false;// нашли ли нужный суффикс
	for (unsigned long long i = 1; n > 0; i++) {
		found = false;
		s_i = to_string(i);
		if (s_i.find_first_not_of("01") < s_i.size()) {
			continue;
		}
		temp = i;
		bin_i = "";
		while (i) {
			remainder = i % 2;
			i /= 2;
			bin_i.insert(0, 1, (char)('0' + remainder));
		}
		i = temp;
		if (bin_i.rfind(s_i) <= bin_i.size() - 1 && bin_i.rfind(s_i) + s_i.size() - 1 == bin_i.size() - 1) {
			// bin_i.rfind(s_i) <= bin_i.size() : проверяем найдена ли подстрока
			// bin_i.rfind(s_i) + s_i.size() - 1 == bin_i.size() - 1 : проверяем, что подстрока явл. суффиксом

			found = true;
		}
		if (found) {
			n--;
		}
	}
	cout << temp << endl;
	return 0;
}

