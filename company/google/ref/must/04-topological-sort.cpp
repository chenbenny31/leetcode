// 04 · Course Schedule (Topological Sort) — LC 207, 210, 2115, 444
// Mother: select/08-graphs/02-course-schedule.cpp
// (1) LC 207 / 210: prereq [a, b] means b -> a; return an order of all, {} if none
// (2) feature rollout: edge [a, b] = a depends on b; name the members of one cycle
//
// clarify:
// direction of [a, b] -> b before a
// dup pairs -> ok
// self-loop {a, a} -> cycle
// several orders / cycles -> any one

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// (1) course order (Kahn)
// core idea: take any course w/ indegree 0, repeat; a course never taken is on a cycle
// LC 207 = order has n courses
// T: O(V + E), S: O(V + E)
vector<int> findOrder(int n, const vector<vector<int>>& prerequisites) {
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

// cycle closed below cur (path from the revisited course to the end), or {}
vector<int> dfs(int cur, const vector<vector<int>>& graph, vector<int>& state,
                vector<int>& path) {
    state[cur] = 1;
    path.push_back(cur);
    for (int nxt : graph[cur]) {
        if (state[nxt] == 1) {
            // nxt is on the path -> cycle = path[pos..end]
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

// (2) name one cycle (three-color DFS)
// core idea: 0 new, 1 on path, 2 done; reaching a 1 = cycle = path from it to the end
// pop the path when a node finishes, or the slice starts too early
// T: O(V + E), S: O(V + E)
vector<int> findCycle(int n, const vector<vector<int>>& edges) {
    // graph[b] = features that depend on b
    vector<vector<int>> graph(n);
    for (const vector<int>& edge : edges) { graph[edge[1]].push_back(edge[0]); }
    // state[v] = 0 new, 1 on current DFS path, 2 done (no cycle reachable)
    vector<int> state(n, 0);
    // path = nodes on the current DFS path, in order
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

// follow-up:
// DFS topo sort -> 3 states, reverse the finish order; 2 states call a diamond a cycle
// smallest order -> min-heap instead of queue
// fewest semesters (LC 1136) -> Kahn by level, count levels
// master sequence / merge ordered lists -> edge per adjacent pair, Kahn on hash maps
// unique order? (LC 444) -> queue never holds 2 courses; also check order == nums
// recipes from supplies (LC 2115) -> Kahn over names, supplies are the sources
// at most k per term (LC 1494) -> bitmask DP for n <= 15, not greedy
// every node on any cycle -> SCC (Tarjan / Kosaraju), keep components of size > 1
