#include <iostream>

using namespace std;


bool isValid(int n)
{
	if (n == 0 || n == 4 || n == 6 || n == 7 || n == 9) {
		return true;
	}
	else {
		return false;
	}
}

int numOfValid(int n) {
	if (n > 0 && n < 4) {
		return 1;
	}
	else if (n == 0) {
		return 0;
	}
	else if (n == 4) {
		return 1;
	}
	else if (n == 6 || n == 5) {
		return 2;
	}
	else if (n == 7) {
		return 3;
	}
	else if (n == 9 || n == 8) {
		return 4;
	}

}


int main()
{

	//long long sumArr[19] = {
	//  1,
	//  5,
	//  25,
	//  125,
	//  625,
	//  3125,
	//  15625,
	//  78125,
	//  390625,
	//  1953125,
	//  9765625,
	//  48828125,
	//  244140625,
	//  1220703125,
	//  6103515625,
	//  30517578125,
	//  152587890625,
	//  762939453125,
	//  3814697265625,
	//};

	//short t;
	//long long n, temp;
	//long long  ans = 0, numOfDigits = 0;
	//int digits[19] = {};
	//cin >> t;

	//while (t--)
	//{
	//	cin >> n;
	//	temp = n;
	//	if (temp >= 0 && temp <= 9) {
	//		if (temp == 0) { ans = 1; }
	//		if (temp == 1) { ans = 1; }
	//		if (temp == 2) { ans = 1; }
	//		if (temp == 3) { ans = 1; }
	//		if (temp == 4) { ans = 2; }
	//		if (temp == 5) { ans = 2; }
	//		if (temp == 6) { ans = 3; }
	//		if (temp == 7) { ans = 4; }
	//		if (temp == 8) { ans = 4; }
	//		if (temp == 9) { ans = 5; }
	//	}
	//	while (temp)
	//	{
	//		digits[numOfDigits] = temp % 10;
	//		temp /= 10;
	//		numOfDigits++;
	//	}


	//	if (numOfDigits > 1)
	//	{
	//		if (digits[numOfDigits - 1] < 4)
	//		{
	//			ans += sumArr[numOfDigits - 1];
	//		}
	//		else
	//		{
	//			for (int i = numOfDigits - 1; i >= 0; i--)
	//			{
	//				if (i != 0) {
	//					ans += (numOfValid(digits[i])) * sumArr[i];
	//				}
	//				if (!isValid(digits[i]) && i != 0) {
	//					break;
	//				}
	//				if (!isValid(digits[i]) && i == 0) {
	//					ans += numOfValid(digits[0]);
	//				}
	//				else if (isValid(digits[i]) && i == 0) {
	//					ans += numOfValid(digits[0]) + 1;
	//				}
	//			}
	//		}
	//	}

	//	cout << ans << endl;

	//	ans = 0;
	//	numOfDigits = 0;
	//}
	int n;
	while (true)
	{
		cin >> n;
		if (n & 1) {
			cout << "last bit is not  equal 1\n";
		}
	}
}