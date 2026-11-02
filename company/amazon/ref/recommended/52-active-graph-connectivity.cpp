// Active nodes + connectivity plan: undirected graph on 0..n-1. Row 0 = sorted nodes
// reachable from start. Then one edge [hub, other] per other component, where hub is
// the smallest node of start's component and other is that component's smallest node,
// components in increasing order of their smallest node. n = 0 -> empty.

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// Label components by scanning nodes in increasing order: BFS from each unlabeled node
// s, so s is the smallest node of its component. T: O(V+E), S: O(V+E).
vector<vector<int>> buildConnectivityPlan(int n, const vector<vector<int>>& edges,
                                          int start) {
    if (n == 0) { return {}; }
    // graph[u] = neighbors of u
    vector<vector<int>> graph(n);
    for (const auto& e : edges) {
        graph[e[0]].push_back(e[1]);
        graph[e[1]].push_back(e[0]);
    }
    // comp[v] = smallest node in v's component (-1 = not labeled yet)
    vector<int> comp(n, -1);
    for (int s = 0; s < n; s++) {
        if (comp[s] != -1) { continue; }
        comp[s] = s;
        queue<int> q;
        q.push(s);
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            for (int nxt : graph[cur]) {
                if (comp[nxt] != -1) { continue; }
                comp[nxt] = s;
                q.push(nxt);
            }
        }
    }
    int hub = comp[start];
    vector<vector<int>> plan;
    // scanning v in order keeps the active list sorted
    vector<int> active;
    for (int v = 0; v < n; v++) {
        if (comp[v] == hub) { active.push_back(v); }
    }
    plan.push_back(active);
    for (int s = 0; s < n; s++) {
        if (comp[s] == s && s != hub) { plan.push_back({hub, s}); }
    }
    return plan;
}

void test(const string& label, int n, const vector<vector<int>>& edges, int start,
          const vector<vector<int>>& want) {
    bool ok = buildConnectivityPlan(n, edges, start) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("hub-not-zero", 4, {{0, 1}, {2, 3}}, 3, {{2, 3}, {2, 0}});
    test("connected", 3, {{0, 1}, {1, 2}}, 2, {{0, 1, 2}});
    test("all-isolated", 3, {}, 1, {{1}, {1, 0}, {1, 2}});
    test("empty", 0, {}, 0, {});
}
