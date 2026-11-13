// LC 207 Course Schedule (Medium) — topological sort: Kahn's BFS / three-color DFS
// numCourses courses 0..n-1, prerequisites [a, b] = take b before a. Return true if all
// courses can be finished (the graph has no cycle).
// Clarify: direction, [a, b] is the edge b -> a; duplicate pairs; a self-loop [a, a] is
//   a cycle; courses with no prerequisites.

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// Core idea: Kahn's BFS: keep taking courses whose indegree is 0 and decrement the
// courses they unlock; a course on or behind a cycle never reaches 0.
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

// true if a course still on the current path (a cycle) is reachable from cur
bool dfs(int cur, const vector<vector<int>>& graph, vector<int>& state) {
    state[cur] = 1;
    for (int nxt : graph[cur]) {
        if (state[nxt] == 1) { return true; }
        if (state[nxt] == 0 && dfs(nxt, graph, state)) { return true; }
    }
    state[cur] = 2;
    return false;
}

// Core idea: DFS from every new course with three states; reaching a course that is on
// the current path is a cycle, while a done course (a diamond) is fine.
// T: O(V+E), S: O(V+E).
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

// follow-up (ideas only):
// - return one valid order (LC 210) -> record Kahn's pop order, {} if fewer than n were
//   taken; with the DFS, the reversed post-order
// - lexicographically smallest order (Google) -> Kahn with a min-heap of ready courses
//   instead of the queue; O(V log V + E)
// - min semesters, no limit per term (LC 1136) -> Kahn level by level: one level = one
//   semester, so the answer is the longest chain; -1 on a cycle
// - at most k courses per term (LC 1494) -> NP-hard in general; bitmask DP over the sets
//   of taken courses for n <= 15, not greedy
// - rebuild the master sequence from ordered subsequences (Google), is nums the only
//   order (LC 444) -> edge per adjacent pair, Kahn; unique iff the queue never holds 2+
// - recipes from supplies (LC 2115) -> Kahn over names: supplies are the sources and a
//   recipe's indegree is its ingredient count; it is made when that reaches 0
// - name one cycle (Google feature rollout) -> three-color DFS keeping the path; reaching
//   an on-path node gives the cycle = path from that node on; pop when a node finishes
// - every node on any cycle -> strongly connected components (Tarjan / Kosaraju): nodes
//   in an SCC of size > 1 or with a self-loop
