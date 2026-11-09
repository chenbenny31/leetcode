// LC 743 Network Delay Time (Medium). Pattern: Dijkstra with a min-heap.
// Directed edges times[i] = [u, v, w] on nodes 1..n. A signal starts at node k; return
// the time for all nodes to receive it, or -1 if some never do.

#include <iostream>
#include <vector>
#include <string>
#include <queue>
#include <utility>
#include <climits>
#include <algorithm>

using std::cout;
using std::string;
using std::vector;
using std::pair;
using std::priority_queue;
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

void test(const string& label, const vector<vector<int>>& times, int n, int k,
          int want) {
    cout << label << (networkDelayTime(times, n, k) == want ? " PASS\n" : " FAIL\n");
}

int main() {
    test("diamond", {{1, 2, 1}, {1, 3, 4}, {2, 4, 2}, {3, 4, 1}}, 4, 1, 4);
    test("shortcut", {{1, 2, 5}, {1, 3, 1}, {3, 2, 1}}, 3, 1, 2);
    test("unreachable", {{1, 2, 1}}, 3, 1, -1);
    test("single", {}, 1, 1, 0);
}

// follow-up: negative edge weights -> Bellman-Ford, n-1 rounds relaxing every edge,
//   O(V*E); an n-th round that still relaxes = negative cycle
// follow-up: dense graph -> O(V^2) array Dijkstra (pick min unvisited each round)
