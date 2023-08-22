#include <iostream>

using namespace std;

int main() {
	// number 1 — 2x
	// number 2 — x^3
	// number 3 — x/3

    int  number;
	double x, z, c, d, f = 0.0;
	cin >> c >> d >> z >> number;
	if (z < 0) {
		x = pow(z, 2) - z;
		cout << "Calculations are carried out under the condition that x < 0" << endl << endl;
	}
	else {
		x = pow(z, 3);
		cout << "Calculations are carried out under the condition that x >= 0" << endl << endl;
	}
	
	double y;
	


	switch (number) {

	case 1:
		f = 2.0 * x;
		cout << "Calculations were performed with the function f = 2 * x" << endl << endl;
	
		
		break;

	case 2:
		f = pow(x, 3);
		cout << "Calculations were performed with the function f = x ^ 3" << endl << endl;
		
		break;

	case 3:
		f = x / 3;
		cout << "Calculations were performed with the function f(x) = x / 3" << endl << endl;		
		break;
	}

	y = pow(sin(c * f + d * d + x * x), 3);
	cout << "y =" << " " << y << endl;
	return 0;
}