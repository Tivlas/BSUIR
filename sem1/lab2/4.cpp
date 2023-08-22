#include <iostream>
#include <cmath>

using namespace std;

int main() {

    double k, x, y;
    cout << "Enter coordinates: k, x, y" << endl;
    cin >> k >> x >> y;
    if (x < 0 || x > k || y < 0 || y > k) {
        cout << "The point lies outside the square" << endl;
    }

    // 1 - top side of square
    // 2 - right side of square
    // 3 - bottom side of square
    // 4 - left side of square

    double d1, d2, d3, d4;
    d1 = abs(y - k);
    d2 = abs(x - k);
    d3 = abs(y);
    d4 = abs(x);

    if (x == 0 || y == 0) {
        cout << 0;
    }

    double cmp = 2e9;
   
    if (cmp > d1) {
        cmp = d1;
    }
    if (cmp > d2) {
        cmp = d2;
    }
    if (cmp > d3) {
        cmp = d3;
    }
    if (cmp > d4) {
        cmp = d4;
    }
    cout << cmp << endl;

    return 0;
}
