#include <iostream>
#include <cmath>
using namespace std;
int main() {

	double A, x, p;
	cin >> x >> p;
	A = x - p;
	double B, h;
	cin >> h;
	B = log(h);
	double K, C, D;
	cin >> K >> C >> D;
	cout << "Y" << " " << "=" << 0.78 * B + ((pow(A, 3)) / (K * C * D));

	return 0;
}