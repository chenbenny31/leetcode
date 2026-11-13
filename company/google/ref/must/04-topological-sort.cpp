// 04 · Course Schedule (Topological Sort) — LC 207, 210, 2115, 444
// Mother problem: select/08-graphs/02-course-schedule.cpp
// Google reports: LC 207 ×3, smallest order ×3, merge subsequences ×2, master sequence
// ×2, feature rollout naming the cycle ×2 (`findCycle`)
//
// LC 207: n courses, prerequisites [a, b] = take b before a; can all be finished?
// Clarify
// - [a, b] means edge b → a → confirm out loud; duplicates fine, a self-loop is a cycle
// Core idea
// - Kahn: indeg[c] = untaken prerequisites; start from every indegree-0 course; take one,
//   decrement what it unlocks, push those that reach 0; order size < n means a cycle.
// - Decrement first, then test == 0. T O(V+E), S O(V+E).
// Tests: diamond → 0..3; 2-cycle off course 0 → {}; self-loop → {}; no prereqs → 0, 1.
//
// Follow-ups
// - return an order (LC 210) → `findOrder` below (LC 207 = order not empty)
// - DFS version → 3 states (new / on path / done); 2 states call a diamond a cycle
// - lexicographically smallest order → min-heap of ready courses instead of the queue
// - fewest semesters (LC 1136) → Kahn level by level; answer = number of levels
// - master sequence / merge ordered lists → edge per adjacent pair, Kahn on hash maps;
//   unique iff the queue never holds 2+ (LC 444: also check order == nums)
// - recipes from supplies (LC 2115) → Kahn over names, supplies are the sources
// - name one dependency cycle → `findCycle` below
// - at most k per term (LC 1494) → bitmask DP for n <= 15; every node on a cycle → SCCs
//
//////////////////////////////////////////////////////////////////////////////////////////

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// Kahn's BFS: keep taking courses with indegree 0, recording the order; a cycle leaves
// some never taken -> {}. LC 207 = !findOrder(...).empty(). T: O(V+E), S: O(V+E).
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
    // courses whose prereqs are all taken
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
    // untaken courses sit on or behind a cycle
    if ((int)order.size() < n) { return {}; }
    return order;
}

// 3-color DFS that also keeps the current path; on reaching a course still on the path,
// returns the path from that course to the end (the cycle), else {}
vector<int> dfs(int cur, const vector<vector<int>>& graph, vector<int>& state,
                vector<int>& path) {
    state[cur] = 1;
    path.push_back(cur);
    for (int nxt : graph[cur]) {
        if (state[nxt] == 1) {
            // nxt is on the path: the cycle is path[pos..end]
            int pos = path.size() - 1;
            while (path[pos] != nxt) { pos--; }
            return vector<int>(path.begin() + pos, path.end());
        }
        if (state[nxt] == 0) {
            vector<int> cycle = dfs(nxt, graph, state, path);
            if (!cycle.empty()) { return cycle; }
        }
    }
    path.pop_back();
    state[cur] = 2;
    return {};
}

// Google (feature rollout): edges [a, b] = feature a depends on b. Name the members of
// one dependency cycle, in edge order starting from the node where the DFS closed it;
// {} if acyclic. 3-color DFS with an explicit path. T: O(V+E), S: O(V+E).
// Pitfall: pop the path when a node finishes, or a done branch's nodes stay on it and
//   the cycle slice starts too early.
vector<int> findCycle(int n, const vector<vector<int>>& edges) {
    // graph[b] = features that depend on b (edge b -> a)
    vector<vector<int>> graph(n);
    for (const vector<int>& edge : edges) { graph[edge[1]].push_back(edge[0]); }
    // state[v]: 0 new, 1 on the current DFS path, 2 done (no cycle reachable)
    vector<int> state(n, 0);
    // nodes on the current DFS path, in order
    vector<int> path;
    for (int v = 0; v < n; v++) {
        if (state[v] != 0) { continue; }
        vector<int> cycle = dfs(v, graph, state, path);
        if (!cycle.empty()) { return cycle; }
    }
    return {};
}

void test(const string& label, int n, const vector<vector<int>>& prerequisites,
          const vector<int>& want) {
    cout << label << (findOrder(n, prerequisites) == want ? " PASS\n" : " FAIL\n");
}

void testCycle(const string& label, int n, const vector<vector<int>>& edges,
               const vector<int>& want) {
    cout << label << (findCycle(n, edges) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, {0, 1, 2, 3});
    test("cycle-off-start", 3, {{2, 1}, {1, 2}}, {});
    test("self-loop", 2, {{1, 1}}, {});
    test("no-prereqs", 2, {}, {0, 1});
    testCycle("cycle-between-tails", 4, {{1, 0}, {2, 1}, {1, 2}, {3, 2}}, {1, 2});
    testCycle("cycle-after-done-branch", 4, {{1, 0}, {2, 0}, {3, 2}, {2, 3}}, {2, 3});
    testCycle("cycle-self-loop", 2, {{1, 1}}, {1});
    testCycle("cycle-none-diamond", 4, {{1, 0}, {2, 0}, {3, 1}, {3, 2}}, {});
}
