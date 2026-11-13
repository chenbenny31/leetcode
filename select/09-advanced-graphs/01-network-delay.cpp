// LC 743 Network Delay Time (Medium) — Dijkstra with a min-heap
// Directed edges times[i] = [u, v, w] on nodes 1..n; a signal starts at node k. Return
// the time until every node has it, or -1 if some node never gets it.
// Clarify: nodes are 1-indexed; weights >= 0; parallel edges; unreachable node gives -1;
//   ranges (n <= 100, w <= 100, so int sums are safe).

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::greater;
using std::max;

// Core idea: Dijkstra from k; with weights >= 0 the first pop of a node is its shortest
// time, and the answer is the largest of them. T: O((V+E) log V), S: O(V+E).
int networkDelayTime(const vector<vector<int>>& times, int n, int k) {
    // graph[u] = (v, w) for each edge u -> v taking w
    vector<vector<pair<int, int>>> graph(n + 1);
    for (const vector<int>& edge : times) {
        graph[edge[0]].push_back({edge[1], edge[2]});
    }
    // dist[v] = best known time from k to v
    vector<int> dist(n + 1, INT_MAX);
    // (time, node), earliest first
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[k] = 0;
    pq.push({0, k});
    while (!pq.empty()) {
        auto [curTime, cur] = pq.top();
        pq.pop();
        // stale entry: cur was already settled with a smaller time
        if (curTime > dist[cur]) { continue; }
        for (const auto& [nxt, delay] : graph[cur]) {
            int nxtTime = curTime + delay;
            if (nxtTime < dist[nxt]) {
                dist[nxt] = nxtTime;
                pq.push({nxtTime, nxt});
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

void test(const string& label, const vector<vector<int>>& times, int n, int k,
          int want) {
    cout << label << (networkDelayTime(times, n, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", {{1, 2, 1}, {1, 3, 4}, {2, 4, 2}, {3, 4, 1}}, 4, 1, 4);
    test("shortcut", {{1, 2, 5}, {1, 3, 1}, {3, 2, 1}}, 3, 1, 2);
    test("unreachable", {{1, 2, 1}}, 3, 1, -1);
    test("single", {}, 1, 1, 0);
    return 0;
}

// follow-up (ideas only):
// - return the path too -> set prev[nxt] = cur whenever dist[nxt] improves, then walk
//   back from the target and reverse
// - at most k stops (LC 787) -> 02-cheapest-flights-k-stops (Bellman-Ford, k + 1 rounds)
// - negative weights -> Bellman-Ford: n - 1 rounds relaxing every edge, O(V*E); an n-th
//   round that still lowers a dist means a negative cycle
// - path with maximum probability (LC 1514) -> same loop with a max-heap and products:
//   best starts at 0.0, the source at 1.0, return on the first pop of end
// - constraint on how you arrive (LC 3970 run of equal labels, stops used) -> put it in
//   the state: dist[node][run], heap of (weight, node, run)
// - weights 0 or 1, e.g. min edge reversals from src to dst (Google) -> 0-1 BFS with a
//   deque: push_front on a 0 edge, push_back on a 1 edge, O(V+E); all weights 1 -> BFS
// - dense graph -> array Dijkstra, pick the closest unsettled node each round, O(V^2)
// - mandatory waypoint w -> dist(s, w) + dist(w, t): Dijkstra from s, and from t on the
//   reversed graph
