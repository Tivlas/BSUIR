#include <iostream>

using namespace std;

int main() {

	double x, y, z;
	cin >> x >> y >> z;
	cout << endl;
	if (x + y > z && x + z > y && z + y > x) {
		cout << "A triangle with such sides  exists" << endl;
	}
	else {
		cout << "A triangle with such sides does not exist" << endl;
	}

	return 0;
}