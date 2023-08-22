
#include <iostream>
#include <cmath>

int main()
{
	int t = 0;
	int L = 0;
	int m = 0;
	std::cout << "Enter t(the number of test cases) ";
	while (!(std::cin >> t) || t < 1 || t>100) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data!\n";
		std::cout << "Reenter t(the number of test cases) ";
	}


	char s1[200];
	char s2[200];
	int res = 0;
	int c = t + 1; // счетчик case #
	while (t) {
		std::cout << "Case #" << c - t << ":" << std::endl;
		std::cout << "Enter L(length of the search string), m(module): ";
		while (!(std::cin >>  L >> m) ||  L < 1 || L>1e9 || m < 1 || m>1e4) {
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			std::cout << "Please check the entered data!\n";
			std::cout << "Reenter L(length of the search string), m(module): ";
		}

		std::cout << "Enter first string: ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get(s1, 100);

		std::cout << "Enter second string: ";
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cin.get(s2, 100);

		int len1 = strlen(s1);
		int len2 = strlen(s2);

		if (len1 > L || len2 > L) {
			std::cout << "Result: " << 0 << std::endl << std::endl;
		}

		else if (len1 + len2 < L) {
			int n = L - (len1 + len2);
			res = (int)pow(26 % m, n) % m;
			std::cout << "Result: " << 2 * res <<  std::endl << std::endl;
		}

		else if (len1 + len2 == L) {
			std::cout << "Reault: " << 2 <<  std::endl << std::endl;
		}

		else {
			bool found = true; // нашли подстроку нужного размера или нет
			for (int i = L - len2; i < len1; i++) {
				if (s1[i] != s2[i - L + len2]) {
					found = false;
				}
			}
			 res = 0;
			if (found) {
				res++;
			}
			found = true;
			for (int i = L - len1; i < len2; i++) {
				if (s2[i] != s1[i - L + len1]) {
					found = false;
				}
			}
			if (found) {
				res++;
			}
			std::cout << "Result: " << res << std::endl << std::endl;
		}
		t--;
	}
	return 0;
}