#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

int n, e;
vector<vector<pair<int, int>>> g;
vector<bool> used;
vector<int> top_sort;

void dfs(int v) {
    used[v] = true;
    for (size_t i = 0; i < g[v].size(); ++i) {
        int to = g[v][i].first;
        if (!used[to]) dfs(to);
    }
    top_sort.push_back(v);
}

void topological_sort() {
    for (int i = 0; i < n; ++i) used[i] = false;
    top_sort.clear();
    for (int i = 0; i < n; ++i)
        if (!used[i]) dfs(i);
    reverse(top_sort.begin(), top_sort.end());
}

int main() {
    cin >> n >> e;
    g.resize(n);
    used.resize(n);
    while (e--) {
        int v, u, l;
        cin >> v >> u >> l;
        g[v].push_back({u, l});
    }

    topological_sort();
    cout << "sort: ";
    for (auto i : top_sort) {
        cout << i << ' ';
    }
    cout << '\n';

    int s, t;
    cin >> s >> t;

    int MIN = numeric_limits<int>::min();
    vector<int> d(n, MIN);
    d[s] = 0;
    vector<int> prev(n, -1);

    for (size_t i = 0; i < n; i++) {
        int u = top_sort[i];
        if (d[u] != MIN) {
            for (auto it = g[u].begin(); it != g[u].end(); it++) {
                if (d[u] + it->second > d[it->first]) {
                    d[it->first] = d[u] + it->second;
                    prev[it->first] = u;
                }
            }
        }
    }

    vector<int> path;
    for (int i = t; i != -1; i = prev[i]) {
        path.push_back(i);
    }
    reverse(path.begin(), path.end());

    cout << "dist from " << s << " to " << t << ": ";
    cout << d[t] << '\n';

    cout << "path: ";
    for (auto i : path) {
        cout << i << ' ';
    }
    cout << '\n';

    return 0;
}