#include <iostream>

using namespace std;

int main() {

	cout << "The program will calculate the sum of even numbers in the range from 1 to the number you entered." << endl;
	cout << "Please, enter number.\n\n";
	int n;
	
	while (!(cin >> n) || n <= 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nPlease check the entered data. Please, enter number. ";
	}
		int i = 2;
		int sum = 0;

		while (i <= n) {
			sum = sum + i;
			i = i + 2;
		}
		cout << "Using a loop: " << sum << endl << endl;
		cout << "Without using a loop: " << n / 2 * (n / 2 + 1) << endl;
	
	return 0;
}