// DAG completion time: task i takes durations[i]; [before, after] means after starts only
// once before finishes. Unlimited tasks run in parallel. Return the time to finish all.
// (Same as LC 2050 with 0-based tasks.)

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

// Kahn's topological order; a task starts when its last prerequisite finishes.
// T: O(V+E), S: O(V+E).
long long minimumCompletionTime(const vector<int>& durations,
                                const vector<vector<int>>& dependencies) {
    int n = durations.size();
    // graph[u] = tasks that wait for u
    vector<vector<int>> graph(n);
    // indeg[v] = prerequisites of v not finished yet
    vector<int> indeg(n, 0);
    for (const auto& d : dependencies) {
        graph[d[0]].push_back(d[1]);
        indeg[d[1]]++;
    }
    // start[v] = latest finish time among v's prerequisites
    vector<long long> start(n, 0);
    queue<int> q;
    for (int v = 0; v < n; v++) {
        if (indeg[v] == 0) { q.push(v); }
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
