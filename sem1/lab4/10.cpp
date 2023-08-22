

#include <iostream>

int main()
{
    int n;
    std::cout << "Enter size of square: ";
    while (!(std::cin >> n) || n <= 0 || n == 2)
    {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        if (n == 2)
        {
            std::cout << "The magic square of the second order does not exist.\n";
            std::cout << "Reenter the size of square to build: ";
        }
        else
        {
            std::cout << "Please check the entered data.\n";
            std::cout << "Reenter the size of square to build: ";
        }
    }

    if (n % 2 == 1)
    {
        int **square = new int *[n * 2 - 1];
        for (int i = 0; i < n * 2 - 1; i++)
        {
            square[i] = new int[n * 2 - 1];
        }
        int cur = 1;
        for (int i = n - 1; i < n * 2 - 1; i++)
        {
            for (int j = 0; j < n; j++)
            {
                square[i - j][j + (i + 1 - n)] = cur;

                if (i - j < (n - 1) / 2)
                { //  (n - 1) / 2) первая строка искомого квдрата; i-j текущая строка
                    square[n + (i - j)][j + (i + 1 - n)] = cur;
                }
                if (i - j > n * 2 - 2 - ((n - 1) / 2))
                { // n * 2 - 2 - ((n - 1) / 2) последняя строка искомого квдрата
                    square[i - j - n][j + (i + 1 - n)] = cur;
                }
                if (j + (i - n + 1) < (n - 1) / 2)
                { // j + (i - n + 1) текущий столбец
                    square[i - j][n + j + (i + 1 - n)] = cur;
                }
                if (j + (i - n + 1) > n * 2 - 2 - ((n - 1) / 2))
                {
                    square[i - j][j + (i + 1 - n) - n] = cur;
                }
                cur++;
            }
        }

        for (int i = (n - 1) / 2; i < n * 2 - 1 - (n - 1) / 2; i++)
        {
            for (int j = (n - 1) / 2; j < n * 2 - 1 - (n - 1) / 2; j++)
            {
                std::cout << square[i][j] << "\t";
            }
            std::cout << std::endl;
        }

        for (int i = 0; i < n * 2 - 1; i++)
        {
            delete[] square[i];
        }
        delete[] square;
    }

    else if (n % 4 == 0)
    {
        int **square = new int *[n];
        for (int i = 0; i < n; i++)
        {
            square[i] = new int[n];
        }
        int cur = 1;
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                square[i][j] = cur;
                cur++;
            }
        }

        for (int i = 0; i < n / 2; i++)
        {
            std::swap(square[i][i], square[n - 1 - i][n - 1 - i]);
            std::swap(square[n - 1 - i][i], square[i][n - 1 - i]);
        }
        for (int i = 4; i < n; i += 4)
        {
            for (int j = 0; j < n - i; j++)
            {
                std::swap(square[i + j][j], square[n - 1 - i - j][n - 1 - j]);
            }
        }
        for (int i = 3; i < n; i += 4)
        {
            for (int j = 0; j <= i; j++)
            {
                std::swap(square[j][i - j], square[n - 1 - j][n - 1 - i + j]);
            }
        }

        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
            {
                std::cout << square[i][j] << "\t";
            }
            std::cout << std::endl;
        }

        for (int i = 0; i < n; i++)
        {
            delete[] square[i];
        }
        delete[] square;
    }
    return 0;
}
