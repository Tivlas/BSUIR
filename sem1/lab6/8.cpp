
#include <iostream>
#include <string>

int main()
{
	int test = 0;// number of tests
	std::cout << "Enter t(the number of test cases) ";
	while (!(std::cin >> test) || test < 1 || test>500) {
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		std::cout << "Please check the entered data!\n";
		std::cout << "Reenter t(the number of test cases) ";
	}
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	std::cout << std::endl;

	std::string s;
	std::string t;

	bool onlyLetters;
	int c = test + 1; //  "case #" counter
	while (test) {
		std::cout << "Case #" << c - test << ":" << std::endl;
		std::cout << "Enter first string: ";
	    onlyLetters = true; // lower case letters
		while (onlyLetters) {
			getline(std::cin, s);
			if (s.size() > 500 || s.size() == 0) {
				std::cout << "The maximum number of letters is 500 and minimum is 1. Reenter the 1st string: ";
			}
			else if (s.find_first_not_of("abcdefghijklmnopqrstuvwxyz") <= s.size()) {
				std::cout << "The string should contain only lower case letters. Reenter the  1st string: ";
			}
			else {
				onlyLetters = false;
			}
		}
		std::cout << "Enter second string: ";
		onlyLetters = true;
		while (onlyLetters) {
			getline(std::cin, t);
			if (t.size() > 2*s.size()-1 || t.size() == 0) {
				std::cout << "The maximum number of letters is 2*(size of s)-1 and minimum is 1. Reenter the 2nd string: ";
			}
			else if (t.find_first_not_of("abcdefghijklmnopqrstuvwxyz") <= t.size()) {
				std::cout << "The string should contain only lower case letters. Reenter the 2nd string: ";
			}
			else {
				onlyLetters = false;
			}
		}

		std::string temp = "";
		bool matchFound = false;
		for (int i = 0; i < (int)(s.size()) && !matchFound; i++) { // s[i] - the element on which we put the chip
			for (int r = 0; r < std::min( (int)(t.size()), (int)(s.size()) - i ); r++) {// to the right

				int l = std::min((int)(t.size()) - r, (int)(s.size()) - i);  // to the left
				temp = "";
				int cur = i;
				for (int k = 0; k < r; k++, cur++) { // k - each step
					temp += s[cur];
				}
				
				for (int k = 0; k < l && cur>=0; k++, cur--) {
					temp += s[cur];
				}

				if (temp == t) {
					matchFound = true;
					break;
				}
			}
		}
		//std::cout << "\n" << temp;
		if (matchFound) {
			std::cout << "YES\n";
		}
		else {
			std::cout << "NO\n";
		}


		test--;
		std::cout << std::endl;
	}
	return 0;
}
