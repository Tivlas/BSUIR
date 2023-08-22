#include <iostream>

using namespace std;

int substraction(int a, int b) {
    while (b) {
        int c = (~a) & b;
        a ^= b;
        b = c << 1;
    }
    return a;
}

void inc(int& i) {  // Нахождение первого 0-го бита и приравнивание его к 1
    int mask = 1;
    while (i & mask) {
        i &= ~mask;
        mask <<= 1;
    }
    i |= mask;
}

int sum(int a, int b) {
    while (b) {
        int c = a & b;  // Нахождение битов, которые равны 1 как в a, так и в b.
        a ^= b;         // Приравниваем их к 0.
        b = c << 1;     // Теперь b равно c * 2, что означает, что те же самые биты в a и b
                        // перемещаются в "более старшие" разряды.
    }
    return a;
}

int multiplication(int a, int b) {
    int res = 0;
    while (b) {       // пока не 0
        if (b & 1) {  // Если b нечетное, добавляем a в res
            res = sum(res, a);
        }
        a <<= 1;  // Таким образом, мы можем умножить a на 2, но разделить b на 2, чтобы сохранить тот же ответ.
        b >>= 1;
    }
    return res;
}

int division(int a, int b) {
    int res = 0;

    while (a >= b) {
        int i = 0, d = b;
        while (a >= (d << 1)) {  // Ищем 2 ^ n, которое помещается в a, n-й бит в результате будет равен 1
            inc(i);
            d <<= 1;
        }
        res |= 1 << i;
        a = substraction(a, d);
    }
    return res;
}

int mod(int a, int b) {
    return substraction(a, multiplication(b, division(a, b)));  // a - b * (a / b)
}

// Var 7


int main() {

    int n;
    cout << "Enter number: ";
    while (!(cin >> n)) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Please check the entered data!\n";
        cout << "Reenter the number: ";
    }

    int prime_numbers[] = { 3, 79, 151 };
    for (int i = 0; i < 3; i++) {
        if (mod(n, prime_numbers[i]) == 0) {
            cout << "The entered number is divisible by " << prime_numbers[i] << endl;
        }
        else {
            cout << "The entered number is NOT divisible by " << prime_numbers[i] << endl;
        }
    }
    return 0;
}