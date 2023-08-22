#include <iostream>
#include <cmath>
using namespace std;

int main() {


	cout << "The program will calculate the value of the function: y = sin(x) - cos(x) on the segment [A;B] at the given points." << endl << endl;


	const double pi = 3.1415;
	int M = 20;
	int A = 0;
	double B = pi / 2;
	double H = (B - A) / M;
	double x = 0;
	int i = 0;
	double y;

	for (x; x < 1.6492; x = i * H) {
		y = sin(x) - cos(x);
		i = i + 1;
		cout << "x:   " << x << endl;
		cout << "y:   " << y << endl << endl;
	}

	return 0;
}