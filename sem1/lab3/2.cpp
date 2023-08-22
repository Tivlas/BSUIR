#include <iostream>

using namespace std;

int main() {

	

	cout << "Let us find the sum of the series whose common term is: (1/2)^n +(1/3)^n" << endl << endl;

	double dn;
	int n = 1;
	double sum = 0;
	double E = 1e-3;
	do {
		dn = 1.0 / pow(2, n) + 1.0 / pow(3, n);
		sum += dn;
		n += 1;
	} while (dn >= E);
	cout << sum << endl;

	return 0;
}