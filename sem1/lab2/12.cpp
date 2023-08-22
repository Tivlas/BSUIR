#include <iostream>

using namespace std;

int main() {

	double x1, y1, r, x2, y2, R, d;
	cin >> x1 >> y1 >> r >> x2 >> y2 >> R;
	d = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
	if (d < abs(R - r)) {
		if (R - r > 0) {
			cout << "Yes";
		}
		else if (R - r < 0) {
			cout << "Yes, but the opposite is true for two figures";
		}
	}
	else if (R > r) {
		if (d > R - r && d < R + r) {
			cout << "Shapes intersect";
		}
	}
	else if (r > R) {
		if (d > r - R && d < R + r) {
			cout << "Shapes intersect";
		}
	}
	else {
		cout << "None of the conditions are met";
	}

	

	return 0;
}