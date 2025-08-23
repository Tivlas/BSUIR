#include <iostream>
#include <limits>
#include <vector>

using namespace std;

int main() {
    int n;
    cin >> n;

    constexpr int mx = numeric_limits<int>::max();

    vector<vector<int>> a(n + 1, vector<int>(n + 1));
    for (int i = 1; i <= n; ++i) {
        for (int j = 1; j <= n; ++j) {
            cin >> a[i][j];
        }
    }

    vector<int> u(n + 1), v(n + 1), p(n + 1), path(n + 1);
    for (int i = 1; i <= n; ++i) {
        p[0] = i;
        int j0 = 0;
        vector<int> minv(n + 1, mx);
        vector<char> used(n + 1, false);
        do {
            used[j0] = true;
            int i0 = p[j0], delta = mx, j1;
            for (int j = 1; j <= n; ++j)
                if (!used[j]) {
                    int cur = a[i0][j] - u[i0] - v[j];
                    if (cur < minv[j]) minv[j] = cur, path[j] = j0;
                    if (minv[j] < delta) delta = minv[j], j1 = j;
                }
            for (int j = 0; j <= n; ++j)
                if (used[j])
                    u[p[j]] += delta, v[j] -= delta;
                else
                    minv[j] -= delta;
            j0 = j1;
        } while (p[j0] != 0);
        do {
            int j1 = path[j0];
            p[j0] = p[j1];
            j0 = j1;
        } while (j0);
    }

    vector<int> ans(n + 1);
    for (int j = 1; j <= n; ++j) ans[p[j]] = j;

    int cost = -v[0];

    for (int i = 1; i <= n; ++i) {
        cout << i << " -> " << ans[i] << '\n';
    }
    cout << cost << '\n';

    return 0;
}
