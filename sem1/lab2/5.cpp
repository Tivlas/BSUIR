#include <iostream>

using namespace std;

int main() {

	double a, b, c, d;
	cin >> a >> b >> c >> d;
	if (c >= d && a < d) {
		cout << "Z =" << " " << a + b / c << endl;
	}
	else if (c < d && a >= d) {
		cout << "Z =" << " " << a - b / c << endl;
	}
	else {
		cout << "Z =" << " " << 0 << endl;
	}


	return 0;
}