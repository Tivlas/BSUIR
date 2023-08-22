
 /*  Сформировать два двумерных динамических массива - матрицы C и D
     размерностью k * k.Размерность массивов(k = 10) ввести с клавиатуры.Значения
     элементов определить согласно выражениям в условии. Вариант 7
     Определить сумму элементов, расположенных на главной и побочной диагоналях,
     для каждой матрицы.На экран вывести полученные массивы и значения сумм.  */




#include <iostream>
#include "Header.h";

int main()
{
    int k = 0;
    std::cout << "Enter k(size of array = k*k)\n";
    while (!(std::cin >> k) || k <= 0) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Please, check the entered data!\n";
        std::cout << "Reenter k(size of array = k*k)\n";
    }

   double** c = new double* [k];
    for (int i = 0; i < k; i++) {
        c[i] = new double[k];
    }
    double** d = new double* [k];
    for (int i = 0; i < k; i++) {
        d[i] = new double[k];
    }

    double mainDiagSum_C = 0.0;
    double sideDiagSum_C = 0.0;
    double mainDiagSum_D = 0.0;
    double sideDiagSum_D = 0.0;

    array_C(c, k, mainDiagSum_C, sideDiagSum_C); 
    array_D(d, k, mainDiagSum_D, sideDiagSum_D);

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            std::cout << c[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << mainDiagSum_C << "  " << sideDiagSum_C << std::endl;

    for (int i = 0; i < k; i++) {
        for (int j = 0; j < k; j++) {
            std::cout << d[i][j] << "\t";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
    std::cout << mainDiagSum_D << "  " << sideDiagSum_D << std::endl;

    for (int i = 0; i < k; i++) {
        delete[] c[i];
    }
    delete[] c;
    for (int i = 0; i < k; i++) {
        delete[] d[i];
    }
    delete[] d;
}

