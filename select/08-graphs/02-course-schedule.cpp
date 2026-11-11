// LC 207 Course Schedule (Medium). Pattern: topological sort (Kahn's BFS).
// numCourses courses 0..n-1, prerequisites [a, b] = take b before a. Can all courses be
// finished? LC 210: return one valid order, or {} if impossible.

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// Kahn's BFS: keep taking courses with indegree 0; a cycle leaves some never taken.
// T: O(V+E), S: O(V+E).
bool canFinish(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks (edge prereq -> course)
    vector<vector<int>> graph(n);
    // indeg[c] = prereqs of c not taken yet
    vector<int> indeg(n, 0);
    for (const vector<int>& req : prerequisites) {
        int course = req[0];
        int prereq = req[1];
        graph[prereq].push_back(course);
        indeg[course]++;
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

// LC 210: same Kahn's BFS, recording the pop order. T: O(V+E), S: O(V+E).
vector<int> findOrder(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks (edge prereq -> course)
    vector<vector<int>> graph(n);
    // indeg[c] = prereqs of c not taken yet
    vector<int> indeg(n, 0);
    for (const vector<int>& req : prerequisites) {
        int course = req[0];
        int prereq = req[1];
        graph[prereq].push_back(course);
        indeg[course]++;
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

// 3-color DFS from cur; true if it reaches a course still on the path (a cycle)
bool dfs(int cur, const vector<vector<int>>& graph, vector<int>& state) {
    state[cur] = 1;
    for (int nxt : graph[cur]) {
        if (state[nxt] == 1) { return true; }
        if (state[nxt] == 0 && dfs(nxt, graph, state)) { return true; }
    }
    state[cur] = 2;
    return false;
}

// 3-color DFS from every unvisited course; reaching a course on the current path is a
// cycle. Reversed post-order would give a valid order. T: O(V+E), S: O(V+E).
bool canFinishDfs(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks (edge prereq -> course)
    vector<vector<int>> graph(n);
    for (const vector<int>& req : prerequisites) {
        int course = req[0];
        int prereq = req[1];
        graph[prereq].push_back(course);
    }
    // state[c]: 0 new, 1 on the current DFS path, 2 done (no cycle reachable)
    vector<int> state(n, 0);
    for (int c = 0; c < n; c++) {
        if (state[c] == 0 && dfs(c, graph, state)) { return false; }
    }
    return true;
}

void test(const string& label, int n, const vector<vector<int>>& prerequisites,
          const vector<int>& want) {
    bool ok = canFinish(n, prerequisites) == !want.empty();
    ok = ok && findOrder(n, prerequisites) == want;
    ok = ok && canFinishDfs(n, prerequisites) == !want.empty();
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, {0, 1, 2, 3});
    test("cycle", 2, {{1, 0}, {0, 1}}, {});
    test("partial-cycle", 3, {{1, 0}, {2, 1}, {1, 2}}, {});
    test("no-prereqs", 2, {}, {0, 1});
    test("self-loop", 2, {{1, 1}}, {});
    test("cycle-off-start", 3, {{2, 1}, {1, 2}}, {});
}

// follow-up: min semesters, no per-term limit (LC 1136) -> Kahn level by level; answer =
//   number of levels (longest path in the DAG)
