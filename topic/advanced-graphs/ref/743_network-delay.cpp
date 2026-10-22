// LC 743 Network Delay Time: directed edges times[i] = [u, v, w] on nodes 1..n. A signal
// starts at node k; return the time for all nodes to receive it, or -1 if some never do.
#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <climits>
#include <algorithm>
using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::greater;
using std::max;

// Dijkstra with a min-heap; the answer is the farthest shortest distance.
// T: O((V+E) log V), S: O(V+E).
int networkDelayTime(vector<vector<int>>& times, int n, int k) {
    vector<vector<pair<int, int>>> graph(n + 1);   // graph[u] = (v, w) for edge u -> v
    for (const auto& t : times) { graph[t[0]].push_back({t[1], t[2]}); }
    vector<int> dist(n + 1, INT_MAX);   // dist[v] = best known time from k to v
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[k] = 0;
    pq.push({0, k});                    // (time, node), earliest first
    while (!pq.empty()) {
        auto [d, cur] = pq.top();
        pq.pop();
        if (d > dist[cur]) { continue; }
        for (const auto& [nxt, w] : graph[cur]) {
            if (d + w < dist[nxt]) {
                dist[nxt] = d + w;
                pq.push({dist[nxt], nxt});
            }
        }
    }
    int result = 0;
    for (int v = 1; v <= n; v++) {
        if (dist[v] == INT_MAX) { return -1; }
        result = max(result, dist[v]);
    }
    return result;
}

void test(const string& label, vector<vector<int>> times, int n, int k, int want) {
    cout << label << (networkDelayTime(times, n, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", {{1, 2, 1}, {1, 3, 4}, {2, 4, 2}, {3, 4, 1}}, 4, 1, 4);
    test("shortcut", {{1, 2, 5}, {1, 3, 1}, {3, 2, 1}}, 3, 1, 2);
    test("unreachable", {{1, 2, 1}}, 3, 1, -1);
    test("single", {}, 1, 1, 0);
}
