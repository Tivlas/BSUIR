#include <iostream>
#include <cmath>


using namespace std;

int main()
{

        int n;
        cout << "How many parts do you want to divide the search area of the root into?" << endl;
        while (!(cin >> n) || n < 0) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nPlease check the entered data. How many parts do you want to divide the search area of the root into? " << endl;
        }
        double step = 1.0 / n;
        double root = 0;
        double cmp = 2e9;
        double fValue = 0;

        for (double i = 2; i <= 3; i += step) {
            fValue = (1.0 / tan(2 * i)) - (1.0 / (1 + pow(i, 2)));
            if (abs(fValue) < cmp) {
                root = i;
                cmp = abs(fValue);

            }
        }
        cout << root << endl;
    

    return 0;
}