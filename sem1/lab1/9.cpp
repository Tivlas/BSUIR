#include <iostream>
using namespace std;
int main() {

	double b1;
	cin >> b1;
	double n = 7, q = 1 / (1 + n),   Sum;
	Sum = b1 / (1 - q);
	cout << Sum; 


	cin >> b1;
	//int t = 15;
	double i = 1 / 16;
	
	double sum;
	sum = b1 / (1 - q);
	cout << sum;
	return 0;
}