#include <iostream>

using namespace std;

int main() {

    int a, b, ab;
    cin >> a >> b;
    ab = a * b;
    
    while (a > 0 && b > 0) {
        if (a > b) {
            a = a - b;
        }
        else  {
            b = b - a;
        }
     } 
   
    if (a <= 0 || b <= 0) {
        cout << "Please, enter correct data.";
    }
    else if (a > b) {
        cout << "GCD: " << a << " " << "LCM: " << ab / a << endl;
    }
    else if (b > a) {
        cout << "GCD: " << b << " " << "LCM: " << ab / b << endl;
        }
    

    return 0;
}