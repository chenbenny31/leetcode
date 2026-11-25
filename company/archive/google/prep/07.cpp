// unsorted logs -> sort by time
// dup pairs are harmless
// multi logs at one time -> count them all

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>
#include <utility>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::sort;
using std::swap;

int find(vector<int>& parent, int x) {
    while (parent[x] != x) {
        parent[x] = parent[parent[x]];
        x = parent[x];
    }
    return x;
}

bool unite(vector<int>& parent, vector<int>& sz, int a, int b) {
    int rootA = find(parent, a);
    int rootB = find(parent, b);
    if (rootA == rootB) { return false; }
    if (sz[rootA] < sz[rootB]) { swap(rootA, rootB); }
    parent[rootB] = rootA;
    sz[rootA] += sz[rootB];
    return true;
}

// sort logs by time, unite in order; one success union mean one group less
int earliestAllConnected(int n, vector<vector<int>> logs) {
    sort(logs.begin(), logs.end());
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    vector<int> sz(n, 1);
    int groups = n;
    for (const vector<int>& log : logs) {
        if (!unite(parent, sz, log[1], log[2])) { continue; }
        
        groups--;
        if (groups == 1) { return log[0]; }
    }
    return -1;
}


bool withinK(int n, const vector<vector<int>>& logs, int cnt, int k) {
    vector<vector<int>> graph(n);
    for (int i = 0; i < cnt; i++) {
        graph[logs[i][1]].push_back(logs[i][2]);
        graph[logs[i][2]].push_back(logs[i][1]);
    }

    for (int src = 0; src < n; src++) {
        vector<int> dist(n, -1); // dist[i] = hops from src, -1 = not reach
        dist[src] = 0;
        queue<int> q;
        q.push(src);
        int reached = 1;

        while (!q.empty()) {
            int cur = q.front();
            q.pop();

            if (dist[cur] == k) { continue; } // exceed k hops
            for (int nxt : graph[cur]) {
                if (dist[nxt] != -1) { continue; }
                dist[nxt] = dist[cur] + 1;
                reached++;
                q.push(nxt);
            }
        }

        if (reached < n) { return false; }
    }
    return true;
}

// a new edge never make a path longeer -> "ok after cnt logs" is monotonic
// -> binary-search the smallest cnt, check with BFS cut at k hops
// T: O(logL * n * (n + L)), S: O(n + L)
int earliestWithinK(int n, vector<vector<int>> logs, int k) {
    sort(logs.begin(), logs.end());
    int total = logs.size();
    int lo = 1; // lo is num of logs, idx should be lo - 1
    int hi = total + 1; // [lo, hi)
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (!withinK(n, logs, mid, k)) { // [ F | T ]
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    if (lo > total) { return -1; } // all !withinK
    return logs[lo - 1][0];
}

void test(const string& label, int n, const vector<vector<int>>& logs, int want) {
    cout << label << (earliestAllConnected(n, logs) == want ? " PASS\n" : " FAIL\n");
}

void testWithinK(const string& label, int n, const vector<vector<int>>& logs, int k,
                 int want) {
    cout << label << (earliestWithinK(n, logs, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    vector<vector<int>> ring = {{1, 0, 1}, {2, 1, 2}, {3, 2, 3}, {5, 3, 0}, {7, 0, 2}};
    test("chain-done-at-3", 4, ring, 3);
    test("unsorted", 4, {{9, 2, 3}, {4, 0, 1}, {6, 1, 2}}, 9);
    test("never", 3, {{1, 0, 1}}, -1);
    test("dup-pair", 3, {{1, 0, 1}, {2, 0, 1}, {3, 1, 2}}, 3);

    testWithinK("k3-chain-is-enough", 4, ring, 3, 3);
    testWithinK("k2-needs-the-ring", 4, ring, 2, 5);
    testWithinK("k1-needs-all-pairs", 4, ring, 1, -1);
    testWithinK("never-connected", 3, {{1, 0, 1}}, 5, -1);
}
