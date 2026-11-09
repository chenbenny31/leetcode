// Cloudflare (JD): Argo Lowest-Latency Route (Medium). Pattern: Dijkstra + parent array.
// PoPs 0..n-1 with directed links (u, v, latencyMs), latencies >= 0. Return the
// lowest-latency route from src to dst as the list of PoPs src..dst; {} if unreachable.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <functional>
#include <algorithm>
#include <climits>

using std::cout;
using std::greater;
using std::pair;
using std::priority_queue;
using std::reverse;
using std::string;
using std::vector;

// This is Dijkstra with a min-heap; parent[] records each PoP's best predecessor and is
// walked back from dst. T: O(E log E), S: O(V + E).
vector<int> lowestLatencyRoute(int n, const vector<vector<int>>& links, int src,
                               int dst) {
    // graph[u] = (v, latencyMs) for each link u -> v
    vector<vector<pair<int, int>>> graph(n);
    for (const auto& link : links) {
        graph[link[0]].push_back({link[1], link[2]});
    }
    // dist[v] = lowest latency found so far from src to v
    vector<int> dist(n, INT_MAX);
    // parent[v] = PoP before v on the best route found so far
    vector<int> parent(n, -1);
    // (latency from src, PoP), lowest latency on top
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<pair<int, int>>> pq;
    dist[src] = 0;
    pq.push({0, src});
    while (!pq.empty()) {
        auto [latency, cur] = pq.top();
        pq.pop();
        // stale entry: cur was already settled with a lower latency
        if (latency > dist[cur]) { continue; }
        for (auto [nxt, linkMs] : graph[cur]) {
            if (latency + linkMs < dist[nxt]) {
                dist[nxt] = latency + linkMs;
                parent[nxt] = cur;
                pq.push({dist[nxt], nxt});
            }
        }
    }
    if (dist[dst] == INT_MAX) { return {}; }
    vector<int> route;
    for (int cur = dst; cur != -1; cur = parent[cur]) {
        route.push_back(cur);
    }
    reverse(route.begin(), route.end());
    return route;
}

void test(const string& label, int n, const vector<vector<int>>& links, int src, int dst,
          const vector<int>& want) {
    vector<int> got = lowestLatencyRoute(n, links, src, dst);
    cout << label << (got == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("detour-beats-direct", 4, {{0, 1, 10}, {1, 3, 10}, {0, 2, 5}, {2, 3, 30},
                                    {0, 3, 50}}, 0, 3, {0, 1, 3});
    test("cheap-first-hop-loses", 4, {{0, 1, 1}, {1, 3, 100}, {0, 2, 20}, {2, 3, 20}},
         0, 3, {0, 2, 3});
    test("one-way-link", 2, {{1, 0, 5}}, 0, 1, {});
    test("src-is-dst", 2, {{0, 1, 5}}, 0, 0, {0});
}

// follow-up: at most k hops -> Bellman-Ford (LC 787): k rounds, each relaxing every link
//   into a copy of last round's dist
// follow-up: equal latency -> prefer fewer hops: heap key and dist hold (latency, hops).
