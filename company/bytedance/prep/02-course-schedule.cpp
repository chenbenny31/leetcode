// edge direction? [a, b] is b->a
// return bool or order? bool/order, {} if empty
// any valid order or special one? e.g. lexi min -> use minHeap over queue
// dup edges, self loop? assume none
// course with no pre-req? valid
// size? n up to 1e5, recursion depth matters

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::reverse;

// Kahn's BFS, T: O(V+E), S: O(V+E)
bool canFinish(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n);
    vector<int> indeg(n, 0);
    for (const auto& p : prereqs) {
        graph[p[1]].push_back(p[0]);
        indeg[p[0]]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { q.push(i); }
    }

    int taken = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        taken++;
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return taken == n; // untaken sites on or behind cycle
}

// follow-up: recording order
vector<int> findOrder(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n); // graph[i] -> courses unblock by i
    vector<int> indeg(n, 0);
    for (const auto& p : prereqs) {
        graph[p[1]].push_back(p[0]);
        indeg[p[0]]++;
    }

    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { q.push(i); }
    }

    vector<int> order;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        order.push_back(cur);
        for (int nxt : graph[cur]) {
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    if ((int)order.size() < n) { return {}; }
    return order;
}

// DFS 3-color
bool dfs(const vector<vector<int>>& graph, int cur, vector<int>& color, vector<int>& order) {
    color[cur] = 1;
    for (int nxt : graph[cur]) {
        if (color[nxt] == 1) { return false; }
        if (color[nxt] == 0) {
            bool ok = dfs(graph, nxt, color, order);
            if (!ok) { return false; }
        }
    }
    color[cur] = 2;
    order.push_back(cur);
    return true;
}

// follow-up: 3-color DFS post-order + rev, T: O(V+E), S: O(V+E)
vector<int> findOrderDFS(int n, const vector<vector<int>>& prereqs) {
    vector<vector<int>> graph(n);
    for (const auto& p : prereqs) {
        graph[p[1]].push_back(p[0]);
    }
    vector<int> color(n, 0);
    vector<int> order;
    for (int i = 0; i < n; i++) {
        if (color[i] != 0) { continue; }
        bool ok = dfs(graph, i, color, order);
        if (!ok) { return {}; }
    }
    reverse(order.begin(), order.end());
    return order;
}

void test(const string& label, int n, const vector<vector<int>>& prerequisites,
          bool want) {
    cout << label << (canFinish(n, prerequisites) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, true);
    test("cycle", 2, {{1, 0}, {0, 1}}, false);
    test("partial-cycle", 3, {{1, 0}, {2, 1}, {1, 2}}, false);
    test("no-prereqs", 1, {}, true);
}
