// 33 · Network Delay Time (Dijkstra) — LC 743, 1514, 3970, 1368
// Mother problem: select/09-advanced-graphs/01-network-delay.cpp
//
// Google reports: max probability, run-limited labels, flights ×2 and train earliest
// arrival (49), direction violations ×3 (`minReversals`, 0-1 BFS)
//
// LC 743: directed edges times[i] = [u, v, w] on nodes 1..n. A signal starts at node k;
// return the time for all nodes to receive it, or -1 if some never do.
//
// Clarify
// - nodes are 1..n → arrays of size n + 1; weights >= 0 (negative → Bellman-Ford)
// - parallel edges fine (cheaper wins); n <= 100, w <= 100 → int sums cannot overflow
//
// Core idea
// - Dijkstra from k with a min-heap of (time, node) (`greater`); first pop is final.
// - Skip stale entries (curTime > dist[cur]); answer = max dist over 1..n.
// - T O((V+E) log V), S O(V+E).
//
// Tests
// - diamond (cheaper path wins); two short edges beat one long; unreachable; single node
//
// Follow-ups
// - direction violations → `minReversals` below: 0-1 BFS, reverse edge costs 1
// - max probability path (LC 1514) → max-heap, multiply, return on the first pop of end
// - label run limit (LC 3970) → Dijkstra over (node, run) states, dist[v][run]
// - valid grid path (LC 1368) → 0-1 BFS on cells: the cell's arrow costs 0, others 1
// - return the path → prev[nxt] = cur on each improvement, walk back from the target
// - at most k stops (LC 787) → Bellman-Ford k + 1 rounds, each from a copy of dist
// - negative weights → Bellman-Ford, n − 1 rounds; an n-th improving round = cycle
// - dense graph → O(V^2) array Dijkstra; waypoint w → dist(s, w) + dist(w, t)

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

// Dijkstra from k; the answer is the farthest shortest distance.
// T: O((V+E) log V), S: O(V+E).
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

// Google: directed edges [u, v] on nodes 0..n-1; walking u -> v is free, walking it
// backwards v -> u costs one violation. Min violations from src to dst, -1 if
// unreachable. 0-1 BFS: cost-0 moves go to the deque front and cost-1 moves to the back,
// so it pops in dist order like Dijkstra without the heap. T: O(V+E), S: O(V+E).
// Pitfall: a node can be improved after it was pushed, so relax on strict < only and
//   let a stale copy re-expand harmlessly (no visited set on push).
int minReversals(int n, const vector<vector<int>>& edges, int src, int dst) {
    // graph[u] = (v, cost): cost 0 along an edge, cost 1 against it
    vector<vector<pair<int, int>>> graph(n);
    for (const auto& edge : edges) {
        graph[edge[0]].push_back({edge[1], 0});
        graph[edge[1]].push_back({edge[0], 1});
    }
    // dist[v] = fewest violations known from src to v
    vector<int> dist(n, INT_MAX);
    // nodes to expand, front ones never farther than back ones
    deque<int> dq;
    dist[src] = 0;
    dq.push_back(src);
    while (!dq.empty()) {
        int cur = dq.front();
        dq.pop_front();
        for (const auto& [nxt, cost] : graph[cur]) {
            int nxtDist = dist[cur] + cost;
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
