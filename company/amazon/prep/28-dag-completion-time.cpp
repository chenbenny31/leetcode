// unliimted parallel workers
// guaranteed DAG (no cycle)
// edge direction [before, after// unliimted parallel workers
// guaranteed DAG (no cycle)
// edge direction [before, after]
// duration range -> could overflow, use long long
// return value -> first time of last week

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::max;

// Kahn's algo for topo order, T: O(V+E), S: O(V+E)
long long minimumCompletionTime(const vector<int>& durations,
                                const vector<vector<int>>& dependencies) {
    int n = durations.size();
    vector<vector<int>> graph(n); // graph[i] -> task deps on i
    vector<int> indeg(n, 0); // pre-req of i
    for (const auto& d : dependencies) {
        int from = d[0];
        int to = d[1];
        graph[from].push_back(to);
        indeg[to]++;
    }

    vector<long long> start(n, 0); // start[i] = last finish time of i's pre-req
    queue<int> q;
    for (int i = 0; i < n; i++) {
        if (indeg[i] == 0) { q.push(i); }
    }

    long long best = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        long long finish = start[cur] + durations[cur];
        best = max(best, finish);

        for (int nxt : graph[cur]) {
            start[nxt] = max(start[nxt], finish);
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    return best;
}

void test(const string& label, const vector<int>& durations,
          const vector<vector<int>>& dependencies, long long want) {
    bool ok = minimumCompletionTime(durations, dependencies) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", {1, 2, 3, 1}, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}, 5);
    test("chain", {1, 2, 3}, {{0, 1}, {1, 2}}, 6);
    test("independent", {5, 1}, {}, 5);
    test("empty", {}, {}, 0);
}
