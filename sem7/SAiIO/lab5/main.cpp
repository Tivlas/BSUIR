#include <iostream>
#include <vector>

using namespace std;

int n, k;
vector<vector<int>> g;
vector<int> rPart;
vector<bool> used;

bool dfs(int v) {
    if (used[v]) return false;
    used[v] = true;
    for (auto to:  g[v]) {
        if (rPart[to] == -1 || dfs(rPart[to])) {
            rPart[to] = v;
            return true;
        }
    }
    return false;
}

int main() {
    cin >> n >> k;
    g.resize(n);

    int u, v;
    for (int i = 0; i < k; ++i) {
        cin >> u >> v;
        g[u-1].push_back(v-1);
    }

    rPart.assign(k, -1);
    for (int v = 0; v < n; ++v) {
        used.assign(n, false);
        dfs(v);
    }

    for (int i = 0; i < k; ++i) {
        if (rPart[i] != -1) {
            cout << rPart[i] + 1 << ' ' << i + 1 << '\n';
        }
    }

    return 0;
}
