#include <iostream>

using namespace std;

int main() {

	setlocale(LC_ALL, "Russian");

	double k;
	cin >> k;                                            
    int x = (int)k;

    if (ceil(k) != floor(k)) {
        cout << "Мы нашли" << " " << k << " " << "гриба в лесу";
    }    
    else if (x >= 10 && x <= 20) {
        cout << "Мы нашли" << " " << k << " " << "грибов в лесу";
    }
    else if (x % 10 >= 2 && x % 10 <= 4) {
        cout << "Мы нашли" << " " << k << " " << "гриба в лесу";
    }
    else if (x % 10 == 1) {
        cout << "Мы нашли" << " " << k << " " << "гриб в лесу";
    }
    else {
        cout << "Мы нашли" << " " << k << " " << "грибов в лесу";
    }

	return 0;
}