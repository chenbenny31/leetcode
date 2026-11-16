// dir of [a,b]: b -> a
// dup pair allows
// self-loop -> cycle
// any order

#include <iostream>
#include <vector>
#include <string>
#include <queue>

using std::cout;
using std::string;
using std::vector;
using std::queue;

// Kahn's, take any indeg==0, repeat and update indeg, non-taken one is in cycle
// T: O(V+E), S: O(V+E)
vector<int> findOrder(int n, const vector<vector<int>>& prerequisites) {
    vector<vector<int>> graph(n); // graph[i] = courses it unlocks
    vector<int> indeg(n, 0);
    for (const auto& req : prerequisites) {
        int to = req[0];
        int from = req[1];
        graph[from].push_back(to);
        indeg[to]++;
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

    if ((int)order.size() < n) { return {}; } // has cycle
    return order;
}

// return cycle reachable from cur, {} means none
vector<int> dfs(int cur, const vector<vector<int>>& graph, vector<int>& state,
                vector<int>& path) {
    state[cur] = 1; // on-path
    path.push_back(cur);

    for (int nxt : graph[cur]) {
        if (state[nxt] == 1) { // cycle = path[pos..end]
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
    state[cur] = 2; // mark visited
    return {};
}

// three color dfs to collect one cycle, T: O(V+E), S: O(V+E)
vector<int> findCycle(int n, const vector<vector<int>>& edges) {
    vector<vector<int>> graph(n); // graph[i] = unlock by i
    for (const auto& e : edges) {
        graph[e[1]].push_back(e[0]);
    }
    vector<int> state(n, 0);
    vector<int> path;
    for (int i = 0; i < n; i++) {
        if (state[i] != 0) { continue; }
        vector<int> cycle = dfs(i, graph, state, path);
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

