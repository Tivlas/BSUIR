


//#pragma region MyRegion
//
//
//
//#include <iostream>
//#include <string>
//#include <windows.h>
//#include <functional>
//#include <regex>
//
//using namespace std;
//
//string Solve(long double a, long double b, long double c) {
//    const long double LD_4 = 4.0L;
//    if(a == 0) {
//        return "Уравнение не является квадратным";
//    }
//    long double D;
//    __asm
//    {
//        FLD b
//        FLD b
//        FMUL
//        FLD LD_4
//        FLD a
//        FLD c
//        FMUL
//        FMUL
//        FSUB
//        FSTP D
//    }
//    cout << "\nДискриминант: " << D << '\n';
//    if(!isfinite(D)) {
//        return "Дискриминант не является конечным числом";
//    }
//    if(D < 0.0L) {
//        return "Нет вещественных корней";
//    }
//    if(D == 0.0L) {
//        long double x;
//        __asm
//        {
//            FLD b
//            FCHS
//            FLD a
//            FLD a
//            FADD
//            FDIV
//            FSTP x
//        }
//        return "x = " + to_string(x);
//    }
//    if(D > 0.0L) {
//        long double x1, x2;
//        __asm
//        {
//            FLD b
//            FCHS
//            FLD D
//            FSQRT
//
//            FSUB
//
//            FLD a
//            FLD a
//            FADD
//            FDIV
//
//            FSTP x1
//        }
//        __asm
//        {
//            FLD b
//            FCHS
//            FLD D
//            FSQRT
//
//            FADD
//
//            FLD a
//            FLD a
//            FADD
//            FDIV
//
//            FSTP x2
//        }
//        return "x1 = " + to_string(x1) + ", " +
//            "x2 = " + to_string(x2);
//    }
//    return "Что-то пошло не так";
//}
//
//template<typename ReturnValue>
//ReturnValue CorrectInput(const string& variable_name, const std::regex& pattern,
//    std::function<ReturnValue(string& input)> converter) {
//
//    string value;
//
//    do {
//        cout << "Введите " << variable_name << ":\t";
//        cin >> value;
//    } while(!regex_match(value, pattern));
//
//    return converter(value);
//
//}
//
//int main() {
//
//    SetConsoleCP(1251);
//
//    SetConsoleOutputCP(1251);
//
//    long double a = CorrectInput<long double>(std::string("коэффициент а"),
//                                              std::regex("-?[0-9]+,?[0-9]*"),
//                                              [](std::string& s) {return atof(s.c_str()); });
//
//    long double b = CorrectInput<long double>(std::string("коэффициент b"),
//                                              std::regex("-?[0-9]+,?[0-9]*"),
//                                              [](std::string& s) {return atof(s.c_str()); });
//
//    long double c = CorrectInput<long double>(std::string("коэффициент c"),
//                                              std::regex("-?[0-9]+,?[0-9]*"),
//                                              [](std::string& s) {return atof(s.c_str()); });
//
//    cout << '\n';
//
//    if(a != 1)
//        cout << ((a == -1) ? '-' : a);
//
//    cout << "x^2 " << ((b < 0) ? '-' : '+') << ' ' << abs(b) << "x "
//        << ((c < 0) ? '-' : '+') << ' ' << abs(c) << " = 0\n";
//
//    cout << '\n' << Solve(a, b, c) << '\n';
//
//    return 0;
//}
//
//#pragma endregion


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

