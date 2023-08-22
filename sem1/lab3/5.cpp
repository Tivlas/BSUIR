#include <iostream>
#include <cmath>

using namespace std;

int main()
{

	int N, n;
	cout << "Enter the number of members in the series and the amount of numbers(x) from which you want to count the function." << endl;
	while (!(cin >> N >> n)) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nCheck the data you entered." << endl;
		cout << "Re-enter the number of members in the series and  the amount of numbers from which you want to count the function." << endl;
	}
	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	double x = 0;
	double sum = 0;
	double fval = 0;
	for (int k = 1; k <= n; k++) {
		cout << "Enter x-" << k << endl;
		while (!(cin >> x) || x < 0.1 || x > 1) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nPlease check the entered data and enter 'x' again" << endl;
		}
		for (int i = 1; i <= N; i++) {
			if (i % 2 == 0) {
				double num = 1;
				for (int j = 0; j < 2 * i + 1; j++) {
					num = num * x;
				}
				sum -= num / (4.0 * i * i - 1.0);
			}
			else {
				double num = 1;
				for (int j = 0; j < 2 * i + 1; j++) {
					num = num * x;
				}
				sum += num / (4.0 * i * i - 1.0);
			}

		}
		cout << "Sum( through a series ): " << sum << endl;
		cout << "Function value: " << ((1 + pow(x, 2)) / 2) * atan(x) - x / 2 << endl << endl;
		sum = 0;
	}


	return 0;
}
