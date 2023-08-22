
#include <iostream>

int main()
{
    srand(time(NULL));
    int n, m;
    std::cout << "Enter n, m(size of array = n * m)\n";
    while (!(std::cin >> n >> m) || n <= 0 || m <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please check the entered data.\n";
        std::cout << "Enter n, m(size of array = n * m)\n";
        std::cout << std::endl;
    }
    std::cout << std::endl;
    
    int** a = new int* [n+2];
    for (int i = 0; i < n+2; i++) {
        a[i] = new int[m+2];
    }
    

    for (int i = 0; i < n + 2; i = n + 1 + i) {
        for (int j = 0; j < m + 2; j++) {
            a[i][j] =INT_MAX;
        }
    }
    for (int j = 0; j < m + 2; j = j + m + 1) {
        for (int i = 0; i < n + 2; i++) {
            a[i][j] =INT_MAX;
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            std::cout << "Enter a[" << i-1 << "][" << j - 1 << "]" << std::endl;
            while (!(std::cin >> a[i][j]) || a[i][j] < 0) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "Please check the entered data.\n";
                std::cout << "Enter a[" << i - 1 << "][" << j - 1 << "]" << std::endl;
            }
        }
    }
  
    for (int i = 1; i <= n; i++) {
        for (int j = 1 ; j <= m; j++) {
            std::cout << a[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    
    int num = 0; // amount
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (a[i][j] < a[i - 1][j] && a[i][j] < a[i - 1][j - 1] && a[i][j] < a[i][j - 1] && a[i][j] <
                a[i][j + 1] && a[i][j] < a[i + 1][j] && a[i][j] < a[i + 1][j + 1] && a[i][j] <
                a[i + 1][j - 1] && a[i][j] < a[i - 1][j + 1]) {
                num++;
            }
        }
    }
    std::cout << std::endl << "Number of local minimums: " << num;

    for (int i = 0; i < n+2; i++) {
        delete[] a[i];
    }
    delete[] a;

    return 0;
}

