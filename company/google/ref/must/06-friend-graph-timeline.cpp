// Friend Graph with Timeline (Google, interview report; part 1 = LC 1101)
// logs[i] = {time, a, b}: a and b become friends at time; people are 0 .. n - 1
// (1) earliest time everyone is connected through friends; -1 if never
// (2) earliest time everyone is connected and every pair is within k hops; -1 if never
// reported: x3 in the 2026 Q4 list; (2) and unfriend logs are the L4 follow-ups
//
// clarify:
// logs sorted by time -> no, sort first
// same pair twice -> harmless
// several logs at one time -> all count at that time
// n == 1 -> assume n >= 2
// (2) hops -> edges on the shortest path

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::queue;
using std::sort;

// root of node's group, w/ path compression
int findRoot(vector<int>& parent, int node) {
    if (parent[node] != node) { parent[node] = findRoot(parent, parent[node]); }
    return parent[node];
}

// (1) earliest time all connected
// core idea: sort logs by time, union in order; a union of 2 roots = 1 group fewer
// T: O(L log L), S: O(n)
int earliestAllConnected(int n, vector<vector<int>> logs) {
    sort(logs.begin(), logs.end());
    // parent[p] = union-find parent of person p
    vector<int> parent(n);
    for (int i = 0; i < n; i++) { parent[i] = i; }
    int groups = n;
    for (const vector<int>& log : logs) {
        int rootA = findRoot(parent, log[1]);
        int rootB = findRoot(parent, log[2]);
        if (rootA == rootB) { continue; }
        parent[rootA] = rootB;
        groups--;
        if (groups == 1) { return log[0]; }
    }
    return -1;
}

// true if, after the first cnt logs, every pair is within k hops
bool withinK(int n, const vector<vector<int>>& logs, int cnt, int k) {
    // graph[p] = friends of p after the first cnt logs
    vector<vector<int>> graph(n);
    for (int i = 0; i < cnt; i++) {
        graph[logs[i][1]].push_back(logs[i][2]);
        graph[logs[i][2]].push_back(logs[i][1]);
    }
    for (int src = 0; src < n; src++) {
        // dist[p] = hops from src, -1 = not reached
        vector<int> dist(n, -1);
        queue<int> q;
        dist[src] = 0;
        q.push(src);
        int reached = 1;
        while (!q.empty()) {
            int cur = q.front();
            q.pop();
            // never go deeper than k hops
            if (dist[cur] == k) { continue; }
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

// (2) earliest time all connected and every pair within k hops
// core idea: a new edge never makes a path longer -> "ok after cnt logs" is monotonic
// binary search the smallest cnt; check = BFS from every person, cut at depth k
// T: O(log L * n * (n + L)), S: O(n + L)
int earliestWithinK(int n, vector<vector<int>> logs, int k) {
    sort(logs.begin(), logs.end());
    int total = logs.size();
    // smallest passing cnt in [1, total]; lo == total + 1 means none
    int lo = 1;
    int hi = total + 1;
    while (lo < hi) {
        int mid = lo + (hi - lo) / 2;
        if (!withinK(n, logs, mid, k)) {
            lo = mid + 1;
        } else {
            hi = mid;
        }
    }
    if (lo > total) { return -1; }
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

// follow-up:
// union by size -> hang the smaller root under the larger; near O(1) per op
// names instead of ids -> map name -> id as names appear
// logs arrive as a stream, "all connected yet?" -> keep parent + groups; O(1) answer
// size of the largest group at any time -> size[] per root, track the max on each union
// unfriend logs (L4 follow-up) -> union-find cannot undo an edge
// unfriend, simple -> keep an edge set; BFS after each log, O(L * (n + L))
// unfriend, all logs known -> replay in reverse if only removals (removal = union)
// unfriend, mixed and fast -> segment tree over time + rollback union-find; mention only
