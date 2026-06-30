// Dijkstra's Algorithm, T: O(ElogV), S: O(V+E)

#include <vector>
#include <queue> // std::priority_queue
#include <utility> // std::pair
#include <functional> // std::greater
#include <climits> // INT_MAX
#include <algorithm> // std::max

class Solution {
public:
    int networkDelayTime(std::vector<std::vector<int>>& times, int n, int k) {
        std::vector<std::vector<std::pair<int, int>>> adj(n + 1);
        for (auto& t : times) { adj[t[0]].push_back({t[1], t[2]}); }

        std::vector<int> dist(n + 1, INT_MAX);
        dist[k] = 0;

        std::vector<std::pair<int, int>> buf;
        buf.reserve(times.size());
        std::priority_queue<std::pair<int, int>,
                            std::vector<std::pair<int, int>>,
                            std::greater<>> pq(std::greater<>{}, std::move(buf));
        pq.push({0, k});

        while (!pq.empty()) {
            auto [d, u] = pq.top(); pq.pop();
            if (d > dist[u]) { continue; } // stale entry

            for (auto& [v, wt] : adj[u]) {
                if (d + wt < dist[v]) {
                    dist[v] = d + wt;
                    pq.push({dist[v], v});
                }
            }
        }

        int best = 0;
        for (int i = 1; i <= n; i++) {
            if (dist[i] == INT_MAX) { return -1; }
            best = std::max(best, dist[i]);
        }
        return best;
    }
};

// d > dist[u]: stale entry skip, avoid reprocessing with outdated distance, dist can only decrease
// std::greater<>: min-heap, Dijkstra requires processing nodes in increasing distance order, access to current closest unvisited node
// 1-indexed adj-list: adj[0] un-used
// final scan: unreachable detection, dist[i] == INT_MAX means node never relaxed
// why not Bellman-Ford: O(VE) works with negative weights, Dijkstra O(ElogV) faster but requires non-negative weights

