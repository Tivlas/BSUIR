#include <iostream>
using namespace std;
int main() {

	int h1, min1, h2, min2;
	cin >> h1 >> min1 >> h2 >> min2;
	int  tm; // time
	tm = h2 * 60 + min2 - h1 * 60 - min1 + 24 * 60;
	tm = tm % (24 * 60);
	cout << tm << " " << "minutes";


	return 0;
}