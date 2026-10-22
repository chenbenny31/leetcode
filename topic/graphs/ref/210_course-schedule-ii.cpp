// LC 210 Course Schedule II: n courses, prerequisite [a, b] = take b before a.
// Return an order that finishes all courses, or an empty list if there is none.
#include <iostream>
#include <vector>
#include <string>
using std::cout;
using std::string;
using std::vector;

// state: 0 = unvisited, 1 = on the current path, 2 = finished (its prereqs are listed)
bool dfs(const vector<vector<int>>& graph, vector<int>& state, int cur,
         vector<int>& order) {
    state[cur] = 1;
    for (int nxt : graph[cur]) {
        // reaching a node on the current path closes a cycle
        if (state[nxt] == 1) { return false; }
        if (state[nxt] == 0 && !dfs(graph, state, nxt, order)) { return false; }
    }
    state[cur] = 2;
    order.push_back(cur);
    return true;
}

// Three-color DFS; post-order lists every course after its prereqs.
// T: O(V+E), S: O(V+E).
vector<int> findOrder(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n);   // graph[a] = prereqs of a
    for (const auto& p : prereqs) { graph[p[0]].push_back(p[1]); }
    vector<int> state(n, 0);
    vector<int> order;              // courses in post-order
    for (int c = 0; c < n; c++) {
        if (state[c] == 0 && !dfs(graph, state, c, order)) { return {}; }
    }
    return order;
}

void test(const string& label, int n, const vector<vector<int>>& prereqs, bool want) {
    vector<int> order = findOrder(n, prereqs);
    bool ok = true;
    if (!want) {
        ok = order.empty();
    } else {
        vector<int> pos(n, -1);   // pos[c] = index of c in the order
        for (int i = 0; i < (int)order.size(); i++) { pos[order[i]] = i; }
        for (int c = 0; c < n; c++) { ok = ok && pos[c] != -1; }
        for (const auto& p : prereqs) { ok = ok && pos[p[1]] < pos[p[0]]; }
    }
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, true);
    test("cycle-behind-root", 4, {{1, 0}, {2, 1}, {3, 2}, {1, 3}}, false);
    test("no-prereqs", 2, {}, true);
}
