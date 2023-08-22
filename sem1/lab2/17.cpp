#include <iostream>

using namespace std;

int main() {

	int W, H;
	cout << "Enter W, H" << endl;
	cin >> W >> H;

	int x1, y1, x2, y2;
	cout << "Enter x1, y1, x2, y2" << endl;
	cin >> x1 >> y1 >> x2 >> y2;

	int w, h;
	cout << "Enter w, h" << endl;
	cin >> w >> h;
	cout << endl;

	int b = x2 - x1;
	int a = y2 - y1;

	if (W < 1 || H <= 0 || x1 < 0 || x2 < 0 ||
		y1 < 0 || y2 < 0 || w < 1 || h < 1 ||
		H > 108 || x1 > W || x2 > W || y1 > H || y2 > H || w > W || h > H) {
		cout << "Please, enter correct data" << endl;
	}
	else {
        
		if (w <= W - x2 || w <= x1 || h <= H - y2 || h <= y1) {
			cout << 0 << endl;
		}
		else if (w + b > W && a + h > H) {
			cout << -1 << endl;
		}
		else if (w + b <= W) {
			if (W - x2 >= x1) {
				cout << x2 - (W - w) << endl;
			}
			else if ( x1 >= W - x2) {
				cout << w - x1 << endl;
			 }
		}
		else if (h > H - y2 && h > y1) {
			if (H - y2 >= y1) {
				cout << y2 - (H - h) << endl;
			}
			else if (H - y2 <= y1) {
				cout << h - y1 << endl;
			}
		}

	}



	return 0;
}