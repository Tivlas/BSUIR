#include <iostream>

using namespace std;

int main() {

	setlocale(LC_ALL, "Russian");

	double k;
	cin >> k;                                            
    int x = (int)k;

    if (ceil(k) != floor(k)) {
        cout << "�� �����" << " " << k << " " << "����� � ����";
    }    
    else if (x >= 10 && x <= 20) {
        cout << "�� �����" << " " << k << " " << "������ � ����";
    }
    else if (x % 10 >= 2 && x % 10 <= 4) {
        cout << "�� �����" << " " << k << " " << "����� � ����";
    }
    else if (x % 10 == 1) {
        cout << "�� �����" << " " << k << " " << "���� � ����";
    }
    else {
        cout << "�� �����" << " " << k << " " << "������ � ����";
    }

	return 0;
}