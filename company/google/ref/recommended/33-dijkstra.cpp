// 33 · Network Delay Time (Dijkstra) — LC 743, 1514, 3970, 1368
// Mother: select/09-advanced-graphs/01-network-delay.cpp
// (1) LC 743: times[i] = {u, v, w}, nodes 1..n, signal from k; time to reach all, or -1
// (2) edges {u, v} on 0..n-1; going against an edge = 1 violation; min src -> dst, or -1
// reported: (2) ×3; also max probability, run-limited labels, flights ×2, train arrival
//
// clarify:
// nodes 1..n or 0..n-1 -> size graph to match
// weights >= 0 -> Dijkstra; negative -> Bellman-Ford
// parallel edges -> ok, adjacency list keeps both
// path sum overflow -> assume fits in int

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <deque>
#include <utility>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
using std::deque;
using std::greater;
using std::max;

// (1) network delay
// core idea: Dijkstra from k w/ min-heap of (time, node); answer = farthest shortest dist
// T: O((V + E) log V), S: O(V + E)
int networkDelayTime(const vector<vector<int>>& times, int n, int k) {
    // graph[u] = (v, w) per edge u -> v taking w
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
        // stale entry: cur already settled w/ smaller time
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

// (2) min reversals
// core idea: 0-1 BFS; cost-0 moves -> deque front, cost-1 -> back; pops in dist order
// T: O(V + E), S: O(V + E)
int minReversals(int n, const vector<vector<int>>& edges, int src, int dst) {
    // graph[u] = (v, cost): cost 0 along an edge, cost 1 against it
    vector<vector<pair<int, int>>> graph(n);
    for (const auto& edge : edges) {
        graph[edge[0]].push_back({edge[1], 0});
        graph[edge[1]].push_back({edge[0], 1});
    }
    // dist[v] = fewest violations known from src to v
    vector<int> dist(n, INT_MAX);
    // nodes to expand; front never farther than back
    deque<int> dq;
    dist[src] = 0;
    dq.push_back(src);
    while (!dq.empty()) {
        int cur = dq.front();
        dq.pop_front();
        for (const auto& [nxt, cost] : graph[cur]) {
            int nxtDist = dist[cur] + cost;
            // strict < only: a node can improve after it was pushed
            if (nxtDist >= dist[nxt]) { continue; }
            dist[nxt] = nxtDist;
            if (cost == 0) {
                dq.push_front(nxt);
            } else {
                dq.push_back(nxt);
            }
        }
    }
    return dist[dst] == INT_MAX ? -1 : dist[dst];
}

void test(const string& label, const vector<vector<int>>& times, int n, int k,
          int want) {
    cout << label << (networkDelayTime(times, n, k) == want ? " PASS\n" : " FAIL\n");
}

void testRev(const string& label, int n, const vector<vector<int>>& edges, int src,
             int dst, int want) {
    bool ok = minReversals(n, edges, src, dst) == want;
    cout << label << (ok ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", {{1, 2, 1}, {1, 3, 4}, {2, 4, 2}, {3, 4, 1}}, 4, 1, 4);
    test("shortcut", {{1, 2, 5}, {1, 3, 1}, {3, 2, 1}}, 3, 1, 2);
    test("unreachable", {{1, 2, 1}}, 3, 1, -1);
    test("single", {}, 1, 1, 0);
    testRev("rev-forward-chain", 3, {{0, 1}, {1, 2}}, 0, 2, 0);
    testRev("rev-one-flip", 3, {{0, 1}, {2, 1}}, 0, 2, 1);
    testRev("rev-long-free-beats-flip", 4, {{3, 0}, {0, 1}, {1, 2}, {2, 3}}, 0, 3, 0);
    testRev("rev-unreachable", 3, {{0, 1}}, 0, 2, -1);
}

// follow-up:
// max probability path (LC 1514) -> max-heap, multiply, return on first pop of end
// label run limit (LC 3970) -> Dijkstra over (node, run) states, dist[v][run]
// valid grid path (LC 1368) -> 0-1 BFS on cells: the cell's arrow costs 0, others 1
// return the path -> prev[nxt] = cur on each improvement, walk back from target
// at most k stops (LC 787) -> Bellman-Ford k + 1 rounds, each from a copy of dist
// negative weights -> Bellman-Ford, n - 1 rounds; an n-th improving round = cycle
// dense graph -> O(V^2) array Dijkstra
// must pass waypoint w -> dist(s, w) + dist(w, t)
