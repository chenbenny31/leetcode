// LC 207 Course Schedule (Medium) — Kahn / three-color DFS
// prereq[i] = {a, b} means b -> a; return if can finish all
//
// clarify:
// dup pairs -> ok
// self-loop {a, a} -> cycle
// n = 0 or no prereq -> true
// need the order? -> follow-up (LC 210)

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// solution 1: Kahn (BFS); also gives the order
// core idea: take any course with indegree 0, repeat; a course never taken is on a cycle
// T: O(V + E), S: O(V + E)
bool canFinish(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks
    vector<vector<int>> graph(n);
    // indeg[c] = prereqs of c not taken yet
    vector<int> indeg(n, 0);
    for (const vector<int>& req : prerequisites) {
        int course = req[0];
        int prereq = req[1];
        graph[prereq].push_back(course);
        indeg[course]++;
    }
    // courses ready to take
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
    return taken == n;
}

// true if a cycle is reachable from cur
bool dfs(int cur, const vector<vector<int>>& graph, vector<int>& state) {
    state[cur] = 1;
    for (int nxt : graph[cur]) {
        if (state[nxt] == 1) { return true; }
        if (state[nxt] == 0 && dfs(nxt, graph, state)) { return true; }
    }
    state[cur] = 2;
    return false;
}

// solution 2: three-color DFS; use it when asked to name the cycle
// core idea: 0 new, 1 on path, 2 done; reaching a 1 = cycle
// 2 colors are not enough: a diamond would look like a cycle
// T: O(V + E), S: O(V + E)
bool canFinishDfs(int n, const vector<vector<int>>& prerequisites) {
    // graph[prereq] = courses it unlocks
    vector<vector<int>> graph(n);
    for (const vector<int>& req : prerequisites) {
        int course = req[0];
        int prereq = req[1];
        graph[prereq].push_back(course);
    }
    vector<int> state(n, 0);
    for (int c = 0; c < n; c++) {
        if (state[c] == 0 && dfs(c, graph, state)) { return false; }
    }
    return true;
}

void test(const string& label, int n, const vector<vector<int>>& prerequisites,
          bool want) {
    bool ok = canFinish(n, prerequisites) == want
              && canFinishDfs(n, prerequisites) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, true);
    test("two-cycle", 2, {{1, 0}, {0, 1}}, false);
    test("cycle-off-start", 3, {{2, 1}, {1, 2}}, false);
    test("self-loop", 2, {{1, 1}}, false);
    return 0;
}

// follow-up:
// return an order (LC 210) -> record cur when popped (snippet below)
// smallest order -> min-heap instead of queue
// fewest semesters (LC 1136) -> Kahn by level, count levels
// at most k per term (LC 1494) -> bitmask DP for n <= 15, not greedy
// unique order? (LC 444) -> edge per adjacent pair; queue never holds 2 courses
// recipes from supplies (LC 2115) -> Kahn over names, supplies are the sources
// name one cycle -> DFS keeps the path, cut at the repeated course
// every node on any cycle -> SCC (Tarjan / Kosaraju)

/* LC 210: the only change to Kahn
   vector<int> order;
   order.push_back(cur);    // right after q.pop()
   if ((int)order.size() < n) { return {}; }
   return order;
*/
