#include <cmath>
#include <iostream>

using namespace std;

int main() {
  
    cout << " The program will find all numbers (less than 10 ^ 6) equal to the sum of their digits" << endl;
    cout << " raised to the power equal to the number of digits." << endl << endl;

  
    for (int i = 1; i < 1000000; i = i + 1) {
        int sum = 0, am = 0, x = i; // am = amount

        while (x != 0) {
            x = x / 10;
            am += 1;  
        }
        x = i;
        while (x != 0) {
            sum = sum + pow(x % 10, am);
            x = x / 10;
        }
        if (sum == i) {
            cout << i << endl;
        }
    }      
         
    return 0;
}