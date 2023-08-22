#include <iostream>

using namespace std;

int main() {

	double a, r, S, C;
	cin >> a >> r;
	S = a * a;
	C = 3.14159265359 * r * r;
	if (S > C) {
		cout << "Square";
	}
	else if (C > S) {
		cout << " Circle";
	}
	else {
		cout << "The areas of the figures are equal";
	}


	return 0;
}