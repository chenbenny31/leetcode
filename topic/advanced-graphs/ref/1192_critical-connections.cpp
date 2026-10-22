// LC 1192 Critical Connections in a Network: n servers 0..n-1 joined by undirected
// connections (connected network, no repeats). Return every critical connection, one
// whose removal disconnects some servers (any order).
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::min;
using std::sort;

// Tarjan bridges with an explicit stack: edge (p, cur) is a bridge when nothing in
// cur's subtree reaches p or above, i.e. low[cur] > disc[p]. T: O(V+E), S: O(V+E).
vector<vector<int>> criticalConnections(int n, vector<vector<int>>& connections) {
    vector<vector<int>> graph(n);   // graph[u] = servers linked to u
    for (const auto& e : connections) {
        graph[e[0]].push_back(e[1]);
        graph[e[1]].push_back(e[0]);
    }
    vector<int> disc(n, -1);   // disc[u] = discovery time, -1 = unvisited
    // low[u] = earliest discovery time reachable from u's subtree
    vector<int> low(n, 0);
    vector<int> parent(n, -1);
    vector<int> next(n, 0);    // next[u] = index of the next neighbour of u to try
    vector<int> stk = {0};     // DFS path from the root to the current server
    int timer = 0;
    disc[0] = low[0] = timer++;
    vector<vector<int>> bridges;
    while (!stk.empty()) {
        int cur = stk.back();
        if (next[cur] < (int)graph[cur].size()) {
            int nxt = graph[cur][next[cur]++];
            if (nxt == parent[cur]) { continue; }
            if (disc[nxt] == -1) {
                parent[nxt] = cur;
                disc[nxt] = low[nxt] = timer++;
                stk.push_back(nxt);
            } else {
                low[cur] = min(low[cur], disc[nxt]);
            }
            continue;
        }
        stk.pop_back();
        if (parent[cur] == -1) { continue; }
        int p = parent[cur];
        low[p] = min(low[p], low[cur]);
        if (low[cur] > disc[p]) { bridges.push_back({p, cur}); }
    }
    return bridges;
}

// edges as (smaller, larger) pairs in sorted order, so results compare directly
vector<vector<int>> normalized(vector<vector<int>> edges) {
    for (auto& e : edges) { sort(e.begin(), e.end()); }
    sort(edges.begin(), edges.end());
    return edges;
}

void test(const string& label, int n, vector<vector<int>> connections,
          const vector<vector<int>>& want) {
    vector<vector<int>> got = criticalConnections(n, connections);
    cout << label << (normalized(got) == normalized(want) ? " PASS\n" : " FAIL\n");
}

int main() {
    test("triangle-plus-tail", 4, {{0, 1}, {1, 2}, {2, 0}, {1, 3}}, {{1, 3}});
    test("all-in-cycle", 3, {{0, 1}, {1, 2}, {2, 0}}, {});
    test("path", 3, {{0, 1}, {1, 2}}, {{0, 1}, {1, 2}});
    test("single-server", 1, {}, {});
}
