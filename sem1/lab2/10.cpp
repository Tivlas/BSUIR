#include <iostream>

using namespace std;

int main() {
	const double pi = 3.14159265359;
	double x;
	cin >> x;
	if (x > 5 * pi / 4) {
		while (x > 5 * pi / 4) {
			x = x - 2 * pi;
		}
	}
	else if (x < -3 * pi / 4) {
		while (x < -3 * pi / 4) {
			x = x + 2 * pi;
		}
	}

	if (x < 0 && x > -0.5787136435) {
		cout << "ln(|x|) = min" << endl;
	}
	else if (x == -0.5787136435) {
		cout << "ln(|x|) = sin(x) = min" << endl;
	}
	else if (x < 1.302964001 && x > 0) {
		cout << "ln(|x|) = min" << endl;
	}
	else if (x == 1.302964001) {
		cout << "ln(|x|) = cos(x) = min" << endl;
	}
	else if (x > (-3 * pi / 4) && x < pi / 4) {
		cout << "sin(x) = min" << endl;
	}
	else if (x < (5 * pi / 4) && x > pi / 4) {
		cout << "cos(x) = min" << endl;
	}
	else if (x == (-3 * pi / 4) || x == (5 * pi / 4)) {
		cout << "cos(x) = sin(x) = min" << endl;
	}


	return 0;
}