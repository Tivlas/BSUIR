#include <cmath>
#include <iostream>

using namespace std;

int main() {

	int n;
	cout << "Enter the number of members in the sequence." << endl;
	while (!(cin >> n) || n <= 0) {
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "\nPlease check the entered data. Enter the number of members of the sequence again.";
	}

	cin.ignore(numeric_limits<streamsize>::max(), '\n');
	int i = 1;
	int sum = 0;
	for (i; i <= n; i++) {
		int a;
		cout << "Enter " << i << " member of sequence: ";
		while (!(cin >> a)) {
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "\nPlease check the entered data.\n";
			cout << "Enter " << i << " member of sequence again: ";
		}


		if (i % 2 == 0) {
			sum -= a << i - 1;
		}
		else {
			sum += a << i - 1;
		}


	}
	cout << sum << endl;


	return 0;
}