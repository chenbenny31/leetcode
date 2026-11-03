// ByteDance (hack2hire) / LC 207: numCourses courses 0..n-1, prerequisites [a, b] =
// take b before a. Can all courses be finished?
// Follow-up LC 210: return one valid order, or {} if impossible (Kahn's and DFS).

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

// Kahn's BFS: repeatedly take a course whose prereqs are all taken; a cycle leaves some
// course never reaching indegree 0. T: O(V+E), S: O(V+E).
bool canFinish(int n, const vector<vector<int>>& prerequisites) {
    // graph[b] = courses that b unlocks (edge b -> a)
    vector<vector<int>> graph(n);
    // indeg[c] = prereqs of c not taken yet
    vector<int> indeg(n, 0);
    for (const auto& p : prerequisites) {
        graph[p[1]].push_back(p[0]);
        indeg[p[0]]++;
    }
    // courses whose prereqs are all taken
    queue<int> q;
    for (int c = 0; c < n; c++) {
        if (indeg[c] == 0) { q.push(c); }
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
    // untaken courses sit on or behind a cycle
    return taken == n;
}

// follow-up LC 210: same Kahn's BFS, recording the pop order. T: O(V+E), S: O(V+E).
vector<int> findOrder(int n, const vector<vector<int>>& prerequisites) {
    // graph[b] = courses that b unlocks (edge b -> a)
    vector<vector<int>> graph(n);
    // indeg[c] = prereqs of c not taken yet
    vector<int> indeg(n, 0);
    for (const auto& p : prerequisites) {
        graph[p[1]].push_back(p[0]);
        indeg[p[0]]++;
    }
    queue<int> q;
    for (int c = 0; c < n; c++) {
        if (indeg[c] == 0) { q.push(c); }
    }
    // courses in the order taken
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

// color: 0 = unvisited, 1 = on the current path, 2 = finished. False on a cycle.
bool dfs(const vector<vector<int>>& graph, int cur, vector<int>& color,
         vector<int>& order) {
    color[cur] = 1;
    for (int nxt : graph[cur]) {
        // edge back to a course still on the path = cycle
        if (color[nxt] == 1) { return false; }
        if (color[nxt] == 0) {
            bool ok = dfs(graph, nxt, color, order);
            if (!ok) { return false; }
        }
    }
    color[cur] = 2;
    // post-order: cur is added after every course it unlocks
    order.push_back(cur);
    return true;
}

// follow-up LC 210: 3-color DFS post-order, reversed. T: O(V+E), S: O(V+E).
vector<int> findOrderDfs(int n, const vector<vector<int>>& prerequisites) {
    // graph[b] = courses that b unlocks (edge b -> a)
    vector<vector<int>> graph(n);
    for (const auto& p : prerequisites) { graph[p[1]].push_back(p[0]); }
    // color[c]: 0 = unvisited, 1 = on the current path, 2 = finished
    vector<int> color(n, 0);
    // courses in post-order (reverse of a valid order)
    vector<int> order;
    for (int c = 0; c < n; c++) {
        if (color[c] != 0) { continue; }
        bool ok = dfs(graph, c, color, order);
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
