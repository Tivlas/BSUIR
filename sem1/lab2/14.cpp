#include <iostream>

using namespace std;

int main() {

	double a, b, c, x, y;
	cout << "Enter a, b, c, x, y" << endl;
	cin >> a >> b >> c >> x >> y;

	if (a <= x && c <= y || c <= x && a <= y || b <= x && c <= y ||
		b <= y && c <= x || a <= x && b <= y || b <= x && a <= y) {
		cout << "Yes";
	}
	else {
		cout << "No";
	}

	return 0;
}