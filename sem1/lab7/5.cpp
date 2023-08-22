
#include <iostream>

using namespace std;

int main()
{
    int n = 0;
    cout << "Enter number to convert to Rome: ";
    while (!(cin >> n) || n < 1 || n > 3999) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        std::cout << "Please check the entered data!\n";
        cout << "Reenter number to convert to Rome: ";
    }
    string res;

    int a[] = { 1,   4,   5,  9,  10,  40,   50,   90,  100, 400,  500, 900, 1000 };
    string b[] = { "I","IV","V","IX","X", "XL", "L", "XC", "C", "CD", "D", "CM", "M" };

    while (n) {
        int i = 12;// индекс последнего элемента массивов a, b
        while (a[i] > n && i > 0) {// ищем индекс нужной строки из массива b
            i--;
        }
        n -= a[i];
        res += b[i];
    }
    cout << res << endl;
    return 0;
}

