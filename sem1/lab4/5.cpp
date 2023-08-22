

#include <iostream>

int main()
{
  
    int n, m;
    std::cout << "Enter n, m(size of array = n * m)\n";
    while (!(std::cin >> n >> m) || n <= 0 || m <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "\nPlease check the entered data.\n";
        std::cout << "Enter n, m(size of array = n * m)\n";
        std::cout << std::endl;
    }
    std::cout << std::endl;

    double** a = new double* [n + 2];
    for (int i = 0; i < n + 2; i++) {
        a[i] = new double[m + 2];
    }

    for (int i = 0; i < n + 2; i = n + 1 + i) {
        for (int j = 0; j < m + 2; j++) {
            a[i][j] = 0.0;
        }
    }
    for (int j = 0; j < m + 2; j = j + m + 1) {
        for (int i = 0; i < n + 2; i++) {
            a[i][j] = 0.0;
        }
    }
  
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            std::cout << "Enter a[" << i - 1 << "][" << j - 1 << "]" << std::endl;
            while (!(std::cin >> a[i][j]) || a[i][j] < 0 || a[i][j] > 1e6) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nPlease check the entered data.\n";
                std::cout << "Enter a[" << i - 1 << "][" << j - 1 << "]" << std::endl;
            }
        }
    }
    std::cout << std::endl;

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            std::cout << a[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;

    double** b = new double* [n];
    for (int i = 0; i < n; i++) {
        b[i] = new double[m];
    }

    double numnei = 8; // number of neighbors
    double sum = 0;
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            if (i == 1 && j == 1 || i == n && j == m || i == 1 && j == m ||
                i == n && j == 1) {
                numnei = 3;
            }
            else if (i == 1 || j == 1 || i == n || j == m) {
                numnei = 5;
            }
            else {
                numnei = 8;
            }
            sum = a[i - 1][j] + a[i - 1][j - 1] + a[i][j - 1] +
                a[i][j + 1] + a[i + 1][j] + a[i + 1][j + 1] +
                a[i + 1][j - 1] + a[i - 1][j + 1];

            b[i - 1][j - 1] = sum / numnei;
        }     
    }
    std::cout << "Result:\n\n";

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            std::cout << b[i][j] << "\t";
        }
        std::cout << std::endl;
    }

    for (int i = 0; i < n+2; i++) {
        delete[] a[i];
    }
    delete[] a;
    for (int i = 0; i < n; i++) {
        delete[] b[i];
    }
    delete[] b;
    return 0;
}
