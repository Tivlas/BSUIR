#include <iostream>
#include <cmath>
using namespace std;

void quick_sort(int* a, int left, int right) {
	int l = left, r = right;
	int mid = a[(r + l) / 2];
	while (l < r) {
		while (a[l] < mid) {
			l++;
		}
		while (a[r] > mid) {
			r--;
		}
		if (l <= r) {
			swap(a[l], a[r]);
			l++; r--;
		}
		if (left < l) {
			quick_sort(a, left, r);
		}
		if (r < right) {
			quick_sort(a, l, right);
		}
	}
}

int main()
{
	int n, k;
	cin >> n >> k;
	int* A = new int[n];
	int M = 0;
	int M_index = 0;
	int diff = 2000000000;
	//ввод
	for (int i = 0; i < n; ++i) {
		cin >> A[i];
	}
	quick_sort(A, 0, n - 1);

	// поиск ближайшего к k
	int len = 0;
	int toLeft = 0;
	int toRight = 0;
	int best_index = 0;
	int best_len = 0;
	int prev_len = 0;
	for (int i = 0; i < n; ++i) {
		if (abs(A[i] - k) <= diff) {
			toLeft = i;
			toRight = n - 1 - i;
			len = min(toLeft, toRight);
			if (len >= prev_len || (i == n - 1 && A[i] != A[i-1])) {
				best_index = i;
				best_len = len;
				prev_len = len;
				M = A[i];
				diff = abs(A[i] - k);
				M_index = i;
			}
		}
	}
	if (M_index == n - 1) {
		cout << 1 << endl;
		cout << n << endl;
	}
	else {
		//int last_m_index = M_index;
		//for (int i = M_index + 1; A[i] == M; ++i) {
		//	++last_m_index;
		//}
		//int len = 0;
		//int toLeft = 0;
		//int toRight = 0;
		//int best_index = 0;
		//int best_len = 0;
		//int prev_len = 0;
		//for (int i = M_index; i <= last_m_index; ++i) {
		//	toLeft = i;
		//	toRight = n - 1 - i;
		//	len = min(toLeft, toRight);
		//	if (len > prev_len) {
		//		best_index = i;
		//		best_len = len;
		//		prev_len = len;
		//	}
		//}
		//// ОТВЕТ
		cout << best_index + best_len - (best_index - best_len) + 1 << endl;
		for (int i = best_index - best_len; i <= best_index + best_len; ++i) {
			cout << i + 1 << " ";
		}
	}
	return 0;
}