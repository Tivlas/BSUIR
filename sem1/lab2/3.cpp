#include <iostream>

using namespace std;

int main() {

        cout << "Enter numbers X and Y" << endl;
        double x, y;
        cin >> x >> y;
        cout << ((x > y) ? x : y) << endl;
        bool ans = (x > y) ? true : false;
        cout << ((ans) ? x : y) << endl;       

        return 0;
   
}