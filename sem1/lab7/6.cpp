
#include <iostream>

using namespace std;


int main()
{
	int test = 0;// number of tests
	cout << "Enter t(the number of test cases) ";
	while (!(std::cin >> test) || test < 1 || test>1000) {
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		cout << "Please check the entered data!\n";
		cout << "Reenter t(the number of test cases) ";
	}

	int n = 0;
	int res = 0;
	int c = test + 1; //  "case #" counter
	while (test) {
		cout << "Case #" << c - test << ":" << endl;
		cout << "Enter n: ";
		while (!(std::cin >> n) || n < 1 || n>1e9) {
			cin.clear();
			cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			cout << "Please check the entered data!\n";
			cout << "Reenter n: ";
		}
		res = (n + 1) / 10;
		cout << "Result: " << res << "\n\n";
		test--;

	}
	cout << endl;
	return 0;
}
