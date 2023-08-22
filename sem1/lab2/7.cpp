#include <iostream>

using namespace std;

int main() {

	cout << " Let's solve an equation of the form: ax^4 + bx^2 + c = 0 " << endl;
	int a, b, c;
	cin >> a >> b >> c;
	double D = pow(b, 2) - 4.0 * a * c;
	if (D < 0) {
		cout << "No solutions!" << endl << endl;
	}
	else  if ((-b + sqrt(D)) / (2.0 * a) >= 0 ) {

		double x1 = sqrt((-b + sqrt(D)) / (2.0 * a));
		cout << x1 << " " << -x1  << endl << endl;

		if ((-b - sqrt(D)) / (2.0 * a) >= 0) {

			double x2 = sqrt((-b - sqrt(D)) / (2.0 * a));
			cout << x2 << " " << -x2 << endl << endl;
		}
	}
	else {
		cout << "No solutions!" << endl << endl;
	}

	// Next
	cout << "Now let's solve an equation of the form: ax^4 + bx^3 + cx^2 + bx + a = 0" << endl;
	cin >> a >> b >> c;
	D = pow(b, 2) - 4.0 * a * (c - 2.0 * a);
	if (D < 0) {
		cout << "No solutions!" << endl << endl;
	}
	else {
		double y1 = (-b + sqrt(D)) / (2.0 * a),
			   y2 = (-b - sqrt(D)) / (2.0 * a);
		D = pow(y1, 2) - 4;
		if (D < 0) {
			cout << "No solutions for y1!" << endl << endl;
		}
		else {
			double x1 = (y1 + sqrt(D)) / (2.0 * a),
			       x2 = (y1 - sqrt(D)) / (2.0 * a);
			cout << x1 << " " << x2 << " " <<  endl << endl;

		}
		D = pow(y2, 2) - 4;
		if (D < 0) {
			cout << "No solutions for y2!" << endl << endl;
		}
		else {
			double x3 = (y2 + sqrt(D)) / (2.0 * a),
				   x4 = (y2 - sqrt(D)) / (2.0 * a);
			cout << x3 << " " << x4 << " " << endl << endl;
		}

	}

	// Next
	cout << "Now let's solve an equation of the form: x^3 + px + q = 0" << endl;
	int p, q;
	cin >> p >> q;
	if (p > 0) {
		cout << "No solutions!" << endl;
	}
	else {


		double alpha = acos((-q / 2.0) * sqrt(pow((3.0 / (-p)), 3))),
			x1 = 2 * sqrt(-p / 3.0) * cos(alpha / 3.0),
			x2 = 2 * sqrt(-p / 3.0) * cos(alpha / 3.0 + 2.0 * acos(-1) / 3.0),
			x3 = 2 * sqrt(-p / 3.0) * cos(alpha / 3.0 - 2.0 * acos(-1) / 3.0);
		cout << x1 << " " << x2 << " " << x3 << endl;
	}


	return 0;
}