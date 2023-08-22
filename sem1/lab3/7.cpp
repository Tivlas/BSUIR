#include <iostream>

using namespace std;

int main() {

	cout << " The program will find a two-digit number that has the property that" << endl;
	cout << " the cube of the sum of its digits is equal to the square of the number itself." << endl << endl;

	int x = 10;

	for (x; x < 99; x++) {
		int a = x % 10;
		int b = x / 10;
		if (pow(a + b, 3) == pow(x, 2)) {
			cout << x << endl;
		}
	
	}
	
	return 0;
}