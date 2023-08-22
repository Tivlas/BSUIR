#include <iostream>
using namespace std;
int main() {

	double v0, t, a, S;
	cin >> v0 >> t >> a;
	S = v0 * t + (a * pow(t, 2)) / 2;
	cout << S << "  " << "meters";


	return 0;
}