#include <iostream>

using namespace std;

int main() {

	double a, b, c;
	cin >> a >> b >> c;
	int N;
	cin >> N;
	switch (N) {

	case 2:
		cout << "Y =" << " " << b * c - pow(a, 2);
		break;

	case 56:
		cout << "Y =" << " " << b * c;
		break;
		
	case 7:
		cout << "Y =" << " " << a * a + c;
		break;

	case 3:
		cout << "Y =" << " " << a - b * c;
		break;

	default:
		cout << "Y =" << " " << pow((a + b), 3);
	}

	return 0;
}