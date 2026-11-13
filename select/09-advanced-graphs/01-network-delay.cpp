// LC 743 Network Delay Time (Medium) — Dijkstra with a min-heap
// directed edges times[i] = [u, v, w], nodes 1..n; signal starts at k
// return time until every node has it, or -1 if some node never gets it
//
// clarify:
// nodes -> 1-indexed
// weights -> >= 0, else Bellman-Ford
// parallel edges -> ok
// unreachable node -> -1
// ranges -> n <= 100, w <= 100, int sums safe

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

// core idea: Dijkstra from k; first pop of a node = its shortest time; ans = max of them
// T: O((V + E) log V), S: O(V + E)
int networkDelayTime(const vector<vector<int>>& times, int n, int k) {
    // graph[u] = (v, w) for each edge u -> v taking w
    vector<vector<pair<int, int>>> graph(n + 1);
    for (const vector<int>& edge : times) {
        graph[edge[0]].push_back({edge[1], edge[2]});
    }
    // dist[v] = best known time k -> v
    vector<int> dist(n + 1, INT_MAX);
    // (time, node), earliest first
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[k] = 0;
    pq.push({0, k});
    while (!pq.empty()) {
        auto [curTime, cur] = pq.top();
        pq.pop();
        // stale entry: cur already settled w/ a smaller time
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

// follow-up:
// return the path -> prev[nxt] = cur when dist[nxt] improves; walk back, reverse
// at most k stops (LC 787) -> 02-cheapest-flights-k-stops (Bellman-Ford, k + 1 rounds)
// negative weights -> Bellman-Ford, n - 1 rounds, O(V * E); n-th round improves = cycle
// max probability (LC 1514), min product path -> heap + products; source = 1.0
// constraint on arrival (LC 3970 run of equal labels) -> add to state: dist[node][run]
// weights 0 / 1 (Google min edge reversals) -> 0-1 BFS w/ deque, O(V + E)
// dense graph -> array Dijkstra, pick closest unsettled each round, O(V^2)
// mandatory waypoint w -> dist(s, w) + dist(w, t); Dijkstra from s, from t on reversed
