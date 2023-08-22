#include <iostream> 
using namespace std;
int main() {

	double x1, y1, x2, y2, x3, y3;
	cin >> x1 >> y1 >> x2 >> y2 >> x3 >> y3;

	double a = sqrt(pow(x1 - x2, 2) + pow(y1 - y2, 2));
	double b = sqrt(pow(x1 - x3, 2) + pow(y1 - y3, 2));
	double c = sqrt(pow(x3 - x2, 2) + pow(y3 - y2, 2));
	cout << "Side lengths:" << "   " << a << "   " << b << "   " << c << endl;

	double p = (a + b + c) / 2, S = sqrt(p * (p - a) * (p - b) * (p - c));
	double Ha = 2 * S / a, 
		   Hb = 2 * S / b,
		   Hc = 2 * S / c;
	cout << "Heights:" <<  "   " << Ha << "   " << Hb << "   " << Hc << endl;

	double Ma = 0.5 * sqrt(2 * b * b + 2 * c * c - a * a);
	double Mb = 0.5 * sqrt(2 * a * a + 2 * c * c - b * b);
	double Mc = 0.5 * sqrt(2 * b * b + 2 * a * a - c * c);
	cout << "Medians:" << "   " << Ma << "   " << Mb << "   " << Mc << endl;

	// Ba is bisector to side a, similarly Bb and Bc
	double Ba = (2 / (b + c)) * sqrt(b * c * p * (p - 1));
	double Bb = (2 / (a + c)) * sqrt(a * c * p * (p - 1));
	double Bc = (2 / (b + a)) * sqrt(b * a * p * (p - 1));
	cout << "Bisectors:" << "   " << Ba << "   " << Bb << "   " << Bc << endl;

	double rdnB = acos((pow(a, 2) + pow(c, 2) - pow(b, 2)) / (2 * a * c));
	double rdnA = acos((pow(b, 2) + pow(c, 2) - pow(a, 2)) / (2 * b * c));
	double rdnC = acos((pow(a, 2) + pow(b, 2) - pow(c, 2)) / (2 * a * b));
	cout << "Radians:" << "   " << rdnA << "   " << rdnB << "   " << rdnC << endl;

	const double pi = 3.141592653589;
	double dgrB = rdnB * 180 / pi;
	double dgrA = rdnA * 180 / pi;
	double dgrC = rdnC * 180 / pi;
	cout << "Degrees:" << "   " << dgrA << "   " << dgrB << "   " << dgrC << endl;

	double R = (a * b * c) / (4 * S);
	double r = S / p;
	cout << "Radius of the circumscribed circle:" << "   " << R << endl;
	cout << "Inscribed circle radius:" << "   " << r << endl;

	double SR = pi * R * R, LR = 2 * pi * R;
	double Sr = pi * r * r, Lr = 2 * pi * r;
	cout << "Circumscribed length:" << "   " << LR << endl;
	cout << "Circumscribed area:" << "   " << SR << endl;
	cout << "Inscribed circle length:" << "   " << Lr << endl;
	cout << "Inscribed area:" << "   " << Sr << endl;

	double prmtr = a + b + c;
	double S1 = a * Ha / 2, S2 = (a * b * c) / (4 * R);
	cout << "Perimeter:" << "   " << prmtr << endl;
	cout << "Area of triangle( searched in three ways )" << "   " << S << "   " << S1 << "   " << S2 << endl;


	return 0;
}