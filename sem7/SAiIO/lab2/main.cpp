#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
using namespace std;

tuple<vector<vector<int>>, vector<vector<int>>, vector<pair<int, int>>> solve(const vector<vector<int>>& A, int P, int Q) {
    int rows = A.size();
    int cols = A[0].size();

    vector<vector<int>> B(rows, vector<int>(cols, -1));
    vector<vector<int>> C(rows, vector<int>(cols, -1));

    for (int p = 0; p < P; ++p) {
        for (int q = 0; q <= Q; ++q) {
            if (p == 0) {
                B[p][q] = A[p][q];
                C[p][q] = q;
            } else {
                int mv = 0;
                int mi = 0;

                for (int i = 0; i <= q; ++i) {
                    int mx = max(A[p][i] + B[p - 1][q - i], mv);

                    if (mx > mv) {
                        mv = mx;
                        mi = i;
                    }
                }

                B[p][q] = mv;
                C[p][q] = mi;
            }
        }
    }

    int q = Q;
    int p = P;
    vector<pair<int, int>> result;
    result.reserve(p);
    while (p > 0) {
        result.push_back({p, C[p - 1][q]});
        q -= C[p - 1][q];
        p -= 1;
    }

    return make_tuple(B, C, result);
}

int main() {
    vector<vector<int>> A = {
        {0, 1, 2, 3},
        {0, 0, 1, 2},
        {0, 2, 2, 3}
    };
    int P = 3;
    int Q = 3;

    auto [B, C, result] = solve(A, P, Q);

    cout << "B:" << '\n';
    for (const auto& row : B) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << '\n';
    }

    cout << "C:" << '\n';
    for (const auto& row : C) {
        for (int val : row) {
            cout << val << " ";
        }
        cout << '\n';
    }

    cout << "Result:" << '\n';
    for (const auto& pair : result) {
        cout << "(" << pair.first << ", " << pair.second << ")" << '\n';
    }

    return 0;
}
