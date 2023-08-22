#include <iostream>
using namespace std;
int main() {

	const double g = 9.81;
	double h, t;
	cin >> h;
	t = sqrt(2 * h / g);
	cout << "Stone fall time =" << " " << t;

	return 0;
}