
#include <iostream>

int main()
{
    srand(time(NULL));
    int N;
    std::cout << "Enter N(size of array = NxN)\n";
    while (!(std::cin >> N) || N <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please check the entered data.\n";
        std::cout << "Enter N(size of array = NxN)\n";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    double** a = new double* [N];
    for (int i = 0; i < N; i++) {
        a[i] = new double[N];
        }

    bool already;
    for (int i = 0; i < N;i++) {
        for (int j = 0; j < N;) {
            already = false;
            double value = (rand()%1000) / 10.0;
            
            for (int cur = 0; cur < i * N + j; cur++) {
                if (a[cur / N][cur % N] == value) {
                    already = true;
                    break;
                }
            }

            if (!already) {
                a[i][j] = value;
                j++;
            }
        }
    }

    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            std::cout << a[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    // поиск строки с наибольшим элементом
    int cur = -1;
    int row = 0; // i
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (a[i][j] > cur) {
                cur = a[i][j];
                row = i;
            }
        }
    }
    std::cout << "row: " << row << std::endl;

    //  поиск столбца с наименьшим элементом
    int cur2 = 2e9;
    int col = 0; // j
    for (int j = 0; j < N; j++) {
        for (int i = 0; i < N; i++) {
            if (a[i][j] < cur2) {
                cur2 = a[i][j];
                col = j;
            }
        }
    }
    std::cout << "column: " << col << std::endl;
    
    double res = 0.0;
    for (int i = 0, j = 0; i < N && j < N; i++, j++){
        res += a[row][j] * a[i][col];
    }
    std::cout << "Dot product of the corresponding column and row = " << res << std::endl;

    for (int i = 0; i < N; i++) {
        delete[] a[i];
    }
    delete[] a;  
    return 0;
}

