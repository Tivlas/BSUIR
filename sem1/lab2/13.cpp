#include <iostream>

using namespace std;

int main() {

	double R, P, Q, r;
	cin >> R >> P >> Q;
	r = P * sin(Q) / 2;
	if (r >= R) {
		cout << "Yes";
	}
	else {
		cout << "No";
	}

	return 0;
}
