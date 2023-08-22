

#include <iostream>

int main()
{
    int n, m;
    std::cout << "Enter the size of minesweeper field(size = n*m): ";
    while (!(std::cin >> n >> m) || n <= 0 || n > 1e6 || m <= 0 || m > 1e6) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Enter the size of minesweeper field: ";
    }
    char** field = new char* [n + 2];
    for (int i = 0; i < n + 2; i++) {
        field[i] = new char[m + 2];
        for (int j = 0; j < m + 2; j++) {
            field[i][j] = '.';
        }
    }
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            std::cout << "Enter the  field[" << i - 1 << "][" << j - 1 << "] element ( '.' - for empty place, '*' - for mine): ";
            while (!(std::cin >> field[i][j]) || (field[i][j] != '.' && field[i][j] != '*')) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                std::cout << "\nPlease check the entered data.\n";
                std::cout << "Enter the  field[" << i - 1 << "][" << j - 1 << "] element ( '.' - for empty place, '*' - for mine): ";
            }
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    }
    
    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            int x[8] = { 1, 1, 1, 0, 0, -1, -1, -1 }; // column
            int y[8] = {-1, 0, 1,-1, 1,  0,  1, -1 }; // row
            if (field[i][j] != '*') {
                char cur = '0';
                for (int k = 0; k < 8; k++) {
                    if (field[i + y[k]][j + x[k]] == '*') {
                        cur++;
                    }
                }
                field[i][j] = cur;
            }
        }
    }

    for (int i = 1; i <= n; i++) {
        for (int j = 1; j <= m; j++) {
            std::cout << field[i][j] << " ";
        }
        std::cout << std::endl;
    }
   
    for (int i = 0; i < n + 2; i++) {
        delete[] field[i];
    }
    delete[] field;
    return 0;
}

