#include <iostream>
using namespace std;
int main() {

	double a1, a100, d, S, n;
	cin >> a1 >> a100;
	d = (a100 - a1) / 99;
	n = 17;
	S = (2 * a1 + (n - 1) * d) / 2 * n;
	cout << "Difference of arithmetic progression:" << " " << d << endl;
	cout << "Sum of n members" << " " << S;


	return 0;
}