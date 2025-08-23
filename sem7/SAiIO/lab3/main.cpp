#include <iostream>
#include <vector>
using namespace std;

pair<int, vector<bool>> solve(int cap, vector<int> v, vector<int> c) {
    if (v.size() != c.size()) {
        throw runtime_error("not equal vector sizes");
    }
    if (v.size() == 0) {
        return {};
    }
    vector<vector<int>> dp(v.size() + 1, vector<int>(cap + 1));
    for (int i = 1; i <= v.size(); i++) {
        for (int j = 1; j <= cap; j++) {
            if (v[i - 1] <= j) {
                dp[i][j] =
                    max(dp[i - 1][j], dp[i - 1][j - v[i - 1]] + c[i - 1]);
            } else {
                dp[i][j] = dp[i - 1][j];
            }
        }
    }

    vector<bool> x(v.size());
    x.back() = v.back() <= cap && (dp[v.size() - 1][cap - v.back()] + c.back() >
                                   dp[v.size() - 1][cap]);
    int vol = x.back() * v.back();
    for (int i = x.size() - 2; i >= 0; i--) {
        int b = cap - vol;
        x[i] = v[i] <= b && (dp[i][b - v[i]] + c[i] > dp[i][b]);
        vol += x[i] * v[i];
    }
    return {dp[v.size()][cap], x};
}

int main() {
    int cap, N;
    cin >> N >> cap;
    vector<int> v(N);
    vector<int> c(N);
    for (auto& i : v) {
        cin >> i;
    }
    for (auto& i : c) {
        cin >> i;
    }
    auto p = solve(cap, v, c);
    cout << p.first << '\n';
    for (bool b : p.second) {
        cout << boolalpha << b << ' ';
    }
    return 0;
}