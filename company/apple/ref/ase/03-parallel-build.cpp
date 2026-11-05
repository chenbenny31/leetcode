// Parallel build / CI pipeline (LC 1136 Parallel Courses + LC 2050 follow-up). Targets
// 0..n-1; deps[i] = {a, b} means a must finish before b starts. Unlimited workers.
// minRounds: fewest rounds if every target takes one round (-1 on a cycle).
// minTotalTime: buildTime[i] per target; earliest time everything is done
// (-1 on a cycle).

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

// Kahn's algorithm one layer at a time: a layer = every target whose deps are all done.
// T: O(V + E), S: O(V + E).
int minRounds(int n, const vector<vector<int>>& deps) {
    // graph[a] = targets that wait for a
    vector<vector<int>> graph(n);
    // indeg[b] = deps of b not finished yet
    vector<int> indeg(n, 0);
    for (const vector<int>& d : deps) {
        graph[d[0]].push_back(d[1]);
        indeg[d[1]]++;
    }
    queue<int> q;
    for (int v = 0; v < n; v++) {
        if (indeg[v] == 0) { q.push(v); }
    }
    int rounds = 0;
    int built = 0;
    while (!q.empty()) {
        rounds++;
        int sz = q.size();
        for (int i = 0; i < sz; i++) {
            int cur = q.front();
            q.pop();
            built++;
            for (int nxt : graph[cur]) {
                indeg[nxt]--;
                if (indeg[nxt] == 0) { q.push(nxt); }
            }
        }
    }
    // targets never built sit on a cycle
    if (built < n) { return -1; }
    return rounds;
}

// Critical path: finish[v] = buildTime[v] + latest finish among v's deps, filled in
// topological order. T: O(V + E), S: O(V + E).
long long minTotalTime(int n, const vector<vector<int>>& deps,
                       const vector<int>& buildTime) {
    vector<vector<int>> graph(n);
    vector<int> indeg(n, 0);
    for (const vector<int>& d : deps) {
        graph[d[0]].push_back(d[1]);
        indeg[d[1]]++;
    }
    // start[v] = earliest time v can start (all deps done)
    vector<long long> start(n, 0);
    queue<int> q;
    for (int v = 0; v < n; v++) {
        if (indeg[v] == 0) { q.push(v); }
    }
    long long total = 0;
    int built = 0;
    while (!q.empty()) {
        int cur = q.front();
        q.pop();
        built++;
        long long finish = start[cur] + buildTime[cur];
        total = max(total, finish);
        for (int nxt : graph[cur]) {
            start[nxt] = max(start[nxt], finish);
            indeg[nxt]--;
            if (indeg[nxt] == 0) { q.push(nxt); }
        }
    }
    if (built < n) { return -1; }
    return total;
}

void test(const string& label, int n, const vector<vector<int>>& deps,
          const vector<int>& buildTime, int wantRounds, long long wantTime) {
    bool ok = minRounds(n, deps) == wantRounds;
    bool okTime = minTotalTime(n, deps, buildTime) == wantTime;
    cout << label << (ok && okTime ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", 4, {{0, 1}, {0, 2}, {1, 3}, {2, 3}}, {1, 5, 2, 1}, 3, 7);
    test("independent", 3, {}, {4, 1, 2}, 1, 4);
    test("cycle", 2, {{0, 1}, {1, 0}}, {1, 1}, -1, -1);
}

// follow-up: at most k workers per round (LC 1494) -> NP-hard in general; bitmask DP
//   over finished sets for n <= 15; in practice greedy by longest remaining path
// follow-up: which targets are on the critical path -> keep parent[v] = the dep that set
//   start[v]; walk back from the target with the largest finish
// follow-up: one target fails -> everything downstream of it is skipped: BFS from it on
//   graph and mark those as blocked
