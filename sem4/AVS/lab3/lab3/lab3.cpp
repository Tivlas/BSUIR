#include <iostream>
#include <iomanip>
#include <windows.h>
#include <math.h>

#include <string>
#include <regex>
#include <functional>

using namespace std;

void Solve(long double a, long double b, long double h, long double eps) {
	cout << '\n';

	cout << "      x          S(x)        Y(x)     abs(S - Y)  n   " << '\n';

	for(long double x = a; x < b + h / 2; x += h) {

		int n = 0;

		long double Y, S, f, p, intp, fracp, trash;

		//Y = pow(exp, cos(x)) * cos(sin(x));
		__asm
		{
			FLD x
			FCOS
			FLDL2E
			FMUL
			FSTP p
		}
		__asm
		{
			FLD p
			FLD1
			FSCALE
			FSTP intp
			FSTP trash
		}
		__asm
		{
			FLD1
			FLD p
			FPREM
			F2XM1
			FADD
			FSTP fracp
		}
		__asm
		{
			FLD intp
			FLD fracp
			FMUL

			FLD x
			FSIN
			FCOS
			FMUL
			FSTP Y
		}
		__asm
		{
			FLD1
			FSTP S

			FLD1
			FSTP f
		}
		while(abs(S - Y) > eps) {
			n++;
			__asm
			{
				FLD f
				FILD n
				FMUL
				FSTP f
			}
			__asm
			{
				FLD S
				FILD n
				FLD x
				FMUL
				FCOS
				FLD f
				FDIV
				FADD
				FSTP S
			}

		}
		cout << setprecision(3) << scientific << showpos <<
			"  " << x << "  " << S << "  " << Y << "  " << abs(S - Y) << "  " <<
			noshowpos << right << setw(2) << n << "  \n";
	}
}



int main() {

	SetConsoleCP(1251);

	SetConsoleOutputCP(1251);


	float a;

	float b;

	float h;

	float eps;

	std::cout << "Enter a: ";
	std::cin >> a;
	std::cout << "Enter b: ";
	std::cin >> b;

	std::cout << "Enter h: ";
	std::cin >> h;

	std::cout << "Enter eps: ";
	std::cin >> eps;

	Solve(a, b, h, eps);

	return 0;
}

