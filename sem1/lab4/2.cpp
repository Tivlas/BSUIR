
#include <iostream>

const int N = 6; // rows
const int M = 6; // columns

// var 7

int main()
{
	if (M == 1 || N == 1) {
		std::cout << "An array with one row or one column has no diagonals. Please correct the entered data.\n";
	}
	else {
		std::cout << "The program will find the maximum element below the side diagonal.\n";
		srand(time(NULL));
		int a[N][M];
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				a[i][j] = rand() % 1000;
			}
		}
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				std::cout << a[i][j] << "\t";
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;

		int cur = -1;
		int k = 0; // i
		int n = 0; // j
		for (int i = 0; i < N; i++) {
			for (int j = M - 1; j > M - 1 - i && j >= 0; j--) {
				std::cout << a[i][j] << "\t";
				if (a[i][j] > cur) {
					cur = a[i][j];
					k = i;
					n = j;
				}
			}
			std::cout << std::endl;
		}
		std::cout << std::endl;
		std::cout << "Maximum element = a[" << k << "][" << n << "] = " << cur << std::endl;
	}
	return 0;
}

